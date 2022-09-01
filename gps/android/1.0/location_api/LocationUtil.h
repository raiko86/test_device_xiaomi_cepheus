/* Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef LOCATION_UTIL_H
#define LOCATION_UTIL_H

#include <GnssDebug.h>
#include <LocationAPI.h>
#include <android/hardware/gnss/1.0/types.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

void convertGnssLocation(Location& in, V1_0::GnssLocation& out);
void convertGnssLocation(const V1_0::GnssLocation& in, Location& out);
void convertGnssConstellationType(GnssSvType& in,
                                  V1_0::GnssConstellationType& out);
void convertGnssSvid(GnssSv& in, int16_t& out);
void convertGnssSvid(GnssMeasurementsData& in, int16_t& out);
void convertGnssEphemerisType(GnssEphemerisType& in,
                              GnssDebug::SatelliteEphemerisType& out);
void convertGnssEphemerisSource(GnssEphemerisSource& in,
                                GnssDebug::SatelliteEphemerisSource& out);
void convertGnssEphemerisHealth(GnssEphemerisHealth& in,
                                GnssDebug::SatelliteEphemerisHealth& out);

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif  // LOCATION_UTIL_H
