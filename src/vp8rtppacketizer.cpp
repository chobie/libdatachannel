/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#if RTC_ENABLE_MEDIA

#include "vp8rtppacketizer.hpp"
#include "impl/internals.hpp"

namespace rtc {

std::vector<std::shared_ptr<rtc::binary>> VP8RtpPacketizer::splitMessage(rtc::binary_ptr message)
{
    /*
     *     0 1 2 3 4 5 6 7
     *    +-+-+-+-+-+-+-+-+
     *    |X|R|N|S|R| PID | (REQUIRED)
     *    +-+-+-+-+-+-+-+-+
     * X: |I|L|T|K| RSV   | (OPTIONAL)
     *    +-+-+-+-+-+-+-+-+
     * I: |M| PictureID   | (OPTIONAL)
     *    +-+-+-+-+-+-+-+-+
     */

    // first byte:
    // uint8_t X = 0b10000000; // Extended control bits present
    uint8_t N = 0b00100000; // Non-reference frame
    uint8_t S = 0b00010000; // Start of VP8 partition
    // R - reserved, always 0
    // assume partition index (PID) is always 0

    // second byte
    //uint8_t I = 0b10000000; // PictureID present

    // third byte
    //uint8_t M = 0b10000000; // PictureID 15-byte extension flag

    static const uint16_t maxDataPartSize = defaultMaximumFragmentSize - 1; // - header length
    auto m = (*message);
    bool isKeyframe = (uint8_t((*message)[0]) & 0b00000001) == 0;
    std::vector<std::shared_ptr<rtc::binary>> split_data;
    std::size_t i = 0;
    do {
        std::size_t len = std::min(message->size() - i, static_cast<std::size_t>(maxDataPartSize));
        uint8_t header_byte = 0;
        if(!isKeyframe) {
            header_byte |= N;
        }
        if(i == 0) {
            header_byte |= S;
        }
        rtc::binary data;
        data.push_back(static_cast<std::byte>(header_byte));
        data.insert(data.end(), message->begin() + i, message->begin() + i + len);

        split_data.emplace_back(std::make_shared<rtc::binary>(std::move(data)));
        i += len;
    } while (i < message->size());
    return split_data;
}

VP8RtpPacketizer::VP8RtpPacketizer(shared_ptr<RtpPacketizationConfig> rtpConfig,
                                   uint16_t maximumFragmentSize)
    : RtpPacketizer(rtpConfig), MediaHandlerRootElement(), maximumFragmentSize(maximumFragmentSize)
    {}

ChainedOutgoingProduct
VP8RtpPacketizer::processOutgoingBinaryMessage(ChainedMessagesProduct messages,
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
