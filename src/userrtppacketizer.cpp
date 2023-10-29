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


UserRtpPacketizer::UserRtpPacketizer(rtcUserPacketizerCallbackFunc cb, shared_ptr<RtpPacketizationConfig> rtpConfig)
    : RtpPacketizer(std::move(rtpConfig)), callback(cb) {}

void UserRtpPacketizer::outgoing(message_vector &messages, [[maybe_unused]] const message_callback &send) {
	message_vector result;
	for(const auto &message : messages) {
        char* packetized = nullptr;
        int size = 0;
        int ret = 1;
        do {
            ret = callback((const char*)message->data(), message->size(), &packetized, &size);
            if (size > 0) {
                auto packet = std::make_shared<binary>(
                    reinterpret_cast<byte*>(packetized), 
                    reinterpret_cast<byte*>(packetized + size));
                if (ret == 1) {
                    result.push_back(packetize(packet, false));
                } else if (ret == 0) {
                    result.push_back(packetize(packet, true));
                } else {
                    break;
                }
            }
        } while (ret > 0);
	}
    

	messages.swap(result);
}

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */