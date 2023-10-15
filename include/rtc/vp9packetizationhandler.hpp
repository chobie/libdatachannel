/**
 * Copyright (c) 2023 Shuhei Taunma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef RTC_VP9_PACKETIZATION_HANDLER_H
#define RTC_VP9_PACKETIZATION_HANDLER_H

#if RTC_ENABLE_MEDIA

#include "vp9rtppacketizer.hpp"
#include "mediachainablehandler.hpp"
#include "nalunit.hpp"

namespace rtc {

/// Handler for VP9 packetization
class RTC_CPP_EXPORT VP9PacketizationHandler final : public MediaChainableHandler {
public:
	/// Construct handler for VP8 packetization.
	/// @param packetizer RTP packetizer for AV1
	VP9PacketizationHandler(shared_ptr<VP9RtpPacketizer> packetizer);
};

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_VP9_PACKETIZATION_HANDLER_H */
