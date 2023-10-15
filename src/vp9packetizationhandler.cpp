/**
 * Copyright (c) 2023 Shuhei Tanuma
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#if RTC_ENABLE_MEDIA

#include "vp9packetizationhandler.hpp"

namespace rtc {

VP9PacketizationHandler::VP9PacketizationHandler(shared_ptr<VP9RtpPacketizer> packetizer)
    : MediaChainableHandler(packetizer) {}

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */
