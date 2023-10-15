/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef RTC_VP8_RTP_PACKETIZER_H
#define RTC_VP8_RTP_PACKETIZER_H

#if RTC_ENABLE_MEDIA

#include "mediahandlerrootelement.hpp"
#include "rtppacketizer.hpp"

namespace rtc {

/// RTP packetization of AV1 payload
class RTC_CPP_EXPORT VP8RtpPacketizer final : public RtpPacketizer, public MediaHandlerRootElement {
	std::vector<std::shared_ptr<rtc::binary>> splitMessage(rtc::binary_ptr message);
	const uint16_t maximumFragmentSize;

public:
	/// Default clock rate for VP8 in RTP
	inline static const uint32_t defaultClockRate = 90 * 1000;
    static const uint16_t defaultMaximumFragmentSize = (RTC_DEFAULT_MTU - 12 - 8 - 40); // SRTP/UDP/IPv6

	/// Constructs VP8 payload packetizer with given RTP configuration.
	/// @note RTP configuration is used in packetization process which may change some configuration
	/// properties such as sequence number.
	/// @param rtpConfig  RTP configuration
	VP8RtpPacketizer(shared_ptr<RtpPacketizationConfig> rtpConfig,
	                 uint16_t maximumFragmentSize = VP8RtpPacketizer::defaultMaximumFragmentSize);

	ChainedOutgoingProduct processOutgoingBinaryMessage(ChainedMessagesProduct messages,
	                                                    message_ptr control) override;

private:
	std::shared_ptr<binary> sequenceHeader;
};

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_VP8_RTP_PACKETIZER_H */
