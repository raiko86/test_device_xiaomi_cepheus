/* Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef LOCATION_UTIL_H
#define LOCATION_UTIL_H

#include <GnssDebug.h>
#include <LocationAPI.h>
#include <android/hardware/gnss/2.0/types.h>
#include <android/hardware/gnss/measurement_corrections/1.0/IMeasurementCorrections.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V2_1 {
namespace implementation {

using MeasurementCorrectionsV1_0 = ::android::hardware::gnss::
    measurement_corrections::V1_0::MeasurementCorrections;
using ::android::hardware::gnss::measurement_corrections::V1_0::
    SingleSatCorrection;

void convertGnssLocation(Location& in, V1_0::GnssLocation& out);
void convertGnssLocation(Location& in, V2_0::GnssLocation& out);
void convertGnssLocation(const V1_0::GnssLocation& in, Location& out);
void convertGnssLocation(const V2_0::GnssLocation& in, Location& out);
void convertGnssConstellationType(GnssSvType& in,
                                  V1_0::GnssConstellationType& out);
void convertGnssConstellationType(GnssSvType& in,
                                  V2_0::GnssConstellationType& out);
void convertGnssSvid(GnssSv& in, int16_t& out);
void convertGnssSvid(GnssMeasurementsData& in, int16_t& out);
void convertGnssEphemerisType(GnssEphemerisType& in,
                              GnssDebug::SatelliteEphemerisType& out);
void convertGnssEphemerisSource(GnssEphemerisSource& in,
                                GnssDebug::SatelliteEphemerisSource& out);
void convertGnssEphemerisHealth(GnssEphemerisHealth& in,
                                GnssDebug::SatelliteEphemerisHealth& out);
bool getCurrentTime(struct timespec& currentTime, int64_t& sinceBootTimeNanos);
void convertSingleSatCorrections(const SingleSatCorrection& in,
                                 GnssSingleSatCorrection& out);
void convertMeasurementCorrections(const MeasurementCorrectionsV1_0& in,
                                   GnssMeasurementCorrections& out);

}  // namespace implementation
}  // namespace V2_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif  // LOCATION_UTIL_H
