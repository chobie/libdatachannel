/**
 * Copyright (c) 2023 Paul-Louis Ageneau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#if RTC_ENABLE_MEDIA

#include <algorithm>

#include "vp9rtppacketizer.hpp"
#include "impl/internals.hpp"

namespace rtc {

std::vector<std::shared_ptr<rtc::binary>> VP9RtpPacketizer::splitMessage(rtc::binary_ptr message)
{
    // should be implment on constructor
    if (!initialized) {
        if (InitialPictureIDFn == nullptr) {
            pictureID = static_cast<uint16_t>(random() % 0x7FFF);
        } else {
            pictureID = InitialPictureIDFn() & 0x7FFF;
        }
        initialized = true;
    }
    
    if (!message || message->empty()) {
        return {};
    }

    static const uint16_t maxDataPartSize = defaultMaximumFragmentSize - vp9HeaderSize;
    auto m = (*message);
    std::vector<std::shared_ptr<rtc::binary>> split_data;
    
    std::size_t payloadDataRemaining = message->size();
    std::size_t payloadDataIndex = 0;

    /* VP9 payload descriptor (from RTP Payload Format for VP9 Video draft16)
                    0 1 2 3 4 5 6 7
                    +-+-+-+-+-+-+-+-+
                    |I|P|L|F|B|E|V|Z| (REQUIRED)
                    +-+-+-+-+-+-+-+-+
               I:   |M| PICTURE ID  | (REQUIRED)
                    +-+-+-+-+-+-+-+-+
               M:   | EXTENDED PID  | (RECOMMENDED)
                    +-+-+-+-+-+-+-+-+
               L:   | TID |U| SID |D| (Conditionally RECOMMENDED)
                    +-+-+-+-+-+-+-+-+                             -\
               P,F: | P_DIFF      |N| (Conditionally REQUIRED)       - up to 3 times
                    +-+-+-+-+-+-+-+-+                             -/
               V:   | SS            |
                    | ..            |
                    +-+-+-+-+-+-+-+-+
    */

    while (payloadDataRemaining > 0) {
        std::size_t currentFragmentSize = std::min((std::size_t)maxDataPartSize, payloadDataRemaining);
        rtc::binary data(vp9HeaderSize + currentFragmentSize);

        // Currently, only implemnts I, M, L.

        // I P L F B E V Z
        // 1 0 0 1 0 0 0 0
        data[0] = (byte)0x90; // F=1, I=1
        if (payloadDataIndex == 0) {
            data[0] |= (byte)0x08; // B=1
        }
        if (payloadDataRemaining - currentFragmentSize <= 0) {
            data[0] |= (byte)0x04; // E=1
        }

        // picture id
        // M PICTUREID
        // 1 
        data[1] = static_cast<std::byte>((pictureID >> 8) | 0x80);
        // extended picture id
        data[2] = static_cast<std::byte>(pictureID & 0xFF);

        std::copy(message->begin() + payloadDataIndex, 
                  message->begin() + payloadDataIndex + currentFragmentSize, 
                  data.begin() + vp9HeaderSize);

        split_data.emplace_back(std::make_shared<rtc::binary>(std::move(data)));
        payloadDataRemaining -= currentFragmentSize;
        payloadDataIndex += currentFragmentSize;
    }

    pictureID++;
    if (pictureID >= 0x8000) {
        pictureID = 0;
    }

    return split_data;
}

VP9RtpPacketizer::VP9RtpPacketizer(shared_ptr<RtpPacketizationConfig> rtpConfig,
                                   uint16_t maximumFragmentSize)
    : RtpPacketizer(rtpConfig), MediaHandlerRootElement(), maximumFragmentSize(maximumFragmentSize)
    {}

ChainedOutgoingProduct
VP9RtpPacketizer::processOutgoingBinaryMessage(ChainedMessagesProduct messages,
                                               message_ptr control) {
    rtc::ChainedMessagesProduct packets = std::make_shared<std::vector<rtc::binary_ptr>>();
    for (auto message : *messages) {
        auto fragments = splitMessage(message);
        if (fragments.size() == 0) {
            return rtc::ChainedOutgoingProduct();
        }
        unsigned i = 0;
        for (; i < fragments.size() - 1; i++) {
            packets->push_back(packetize(fragments[i], false));
        }
        packets->push_back(packetize(fragments[i], true));
    }
    return {packets, control};
}

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */
