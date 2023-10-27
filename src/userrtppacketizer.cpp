/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#if RTC_ENABLE_MEDIA

#include "userrtppacketizer.hpp"

#include "impl/internals.hpp"

#include <cassert>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace rtc {


UserRtpPacketizer::UserRtpPacketizer(rtcUserPacketizerCallbackFunc cb,
                                     shared_ptr<RtpPacketizationConfig> rtpConfig,
                                     uint16_t maximumFragmentSize)
    : RtpPacketizer(rtpConfig), 
    MediaHandlerRootElement(), 
    callback(cb),
    maximumFragmentSize(maximumFragmentSize){}

ChainedOutgoingProduct
UserRtpPacketizer::processOutgoingBinaryMessage(ChainedMessagesProduct messages,
                                                message_ptr control) {
	ChainedMessagesProduct packets = std::make_shared<std::vector<binary_ptr>>();
	for (auto message : *messages) {
        char* result = nullptr;
        int size = 0;
        int num_packets = 0;
        int ret = 1;
        do {
            ret = callback((const char*)message->data(), message->size(), &result, &size);
            if (size > 0) {
                auto packet = std::make_shared<binary>(
                    reinterpret_cast<byte*>(result), 
                    reinterpret_cast<byte*>(result + size));
                if (ret == 1) {
                    packets->push_back(packetize(packet, false));
                    num_packets++;
                } else if (ret == 0) {
                    packets->push_back(packetize(packet, true));
                    num_packets++;
                } else {
                    break;
                }
            }
        } while (ret > 0);

        if (num_packets == 0) {
            return rtc::ChainedOutgoingProduct();
        }
	}

	return {packets, control};
}

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */
