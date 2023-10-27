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

#include "mediahandlerrootelement.hpp"
#include "rtppacketizer.hpp"

namespace rtc {

/// RTP packetization of User payload
class RTC_CPP_EXPORT UserRtpPacketizer final : public RtpPacketizer,
                                               public MediaHandlerRootElement {

	rtcUserPacketizerCallbackFunc callback;
public:
	static const uint16_t defaultMaximumFragmentSize = 8192;
	int maximumFragmentSize;
	
	UserRtpPacketizer(rtcUserPacketizerCallbackFunc cb, shared_ptr<RtpPacketizationConfig> rtpConfig,
	                  uint16_t maximumFragmentSize = UserRtpPacketizer::defaultMaximumFragmentSize);

	ChainedOutgoingProduct processOutgoingBinaryMessage(ChainedMessagesProduct messages,
	                                                    message_ptr control) override;
};

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_USER_RTP_PACKETIZER_H */
