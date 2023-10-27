/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef RTC_USER_PACKETIZATION_HANDLER_H
#define RTC_USER_PACKETIZATION_HANDLER_H

#if RTC_ENABLE_MEDIA

#include "userrtppacketizer.hpp"
#include "mediachainablehandler.hpp"

namespace rtc {

/// Handler for User packetization
class RTC_CPP_EXPORT UserPacketizationHandler final : public MediaChainableHandler {
public:
	/// Construct handler for H264 packetization.
	/// @param packetizer RTP packetizer for h264
	UserPacketizationHandler(shared_ptr<UserRtpPacketizer> packetizer);
};

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */

#endif /* RTC_USER_PACKETIZATION_HANDLER_H */
