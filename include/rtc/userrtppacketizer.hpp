/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef RTC_USER_RTP_PACKETIZER_H
#define RTC_USER_RTP_PACKETIZER_H

#if RTC_ENABLE_MEDIA

#include "rtppacketizer.hpp"

namespace rtc {

/// RTP packetization for User Payload
class RTC_CPP_EXPORT UserRtpPacketizer final : public RtpPacketizer {
public:
	rtcUserPacketizerCallbackFunc callback;

	UserRtpPacketizer(rtcUserPacketizerCallbackFunc cb, shared_ptr<RtpPacketizationConfig> rtpConfig);

	void outgoing(message_vector &messages, const message_callback &send) override;

private:
};

// For backward compatibility, do not use
using UserRtpPacketizationHandler [[deprecated("Add UserRtpPacketizer directly")]] = PacketizationHandler;

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_USER_RTP_PACKETIZER_H */