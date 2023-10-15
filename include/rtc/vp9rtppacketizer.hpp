/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef RTC_VP9_RTP_PACKETIZER_H
#define RTC_VP9_RTP_PACKETIZER_H

#if RTC_ENABLE_MEDIA

#include "mediahandlerrootelement.hpp"
#include "rtppacketizer.hpp"

namespace rtc {

/// RTP packetization of VP9 payload
class RTC_CPP_EXPORT VP9RtpPacketizer final : public RtpPacketizer, public MediaHandlerRootElement {
	std::vector<std::shared_ptr<rtc::binary>> splitMessage(rtc::binary_ptr message);
	const uint16_t maximumFragmentSize;
	bool initialized = false;
	uint16_t pictureID = 0;
	std::function<uint16_t()> InitialPictureIDFn = nullptr;
	static const uint16_t vp9HeaderSize = 3;

public:
	/// Default clock rate for VP9 in RTP
	inline static const uint32_t defaultClockRate = 90 * 1000;
    static const uint16_t defaultMaximumFragmentSize = (RTC_DEFAULT_MTU - 12 - 8 - 40); // SRTP/UDP/IPv6

	/// Constructs VP9 payload packetizer with given RTP configuration.
	/// @note RTP configuration is used in packetization process which may change some configuration
	/// properties such as sequence number.
	/// @param rtpConfig  RTP configuration
	VP9RtpPacketizer(shared_ptr<RtpPacketizationConfig> rtpConfig,
	                 uint16_t maximumFragmentSize = VP9RtpPacketizer::defaultMaximumFragmentSize);

	ChainedOutgoingProduct processOutgoingBinaryMessage(ChainedMessagesProduct messages,
	                                                    message_ptr control) override;

private:
	std::shared_ptr<binary> sequenceHeader;
};

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_VP9_RTP_PACKETIZER_H */
