/* Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation, nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <LocationUtil.h>
#include <log_util.h>
#include <inttypes.h>
#include <loc_misc_utils.h>
#include <gps_extended_c.h>
namespace android
{
    namespace hardware
    {
        namespace gnss
        {
            namespace V2_1
            {
                namespace implementation
                {
                    using ::android::hardware::gnss::measurement_corrections::V1_0::GnssSingleSatCorrectionFlags;
                    using ::android::hardware::gnss::V1_0::GnssLocationFlags;
                    using ::android::hardware::gnss::V2_0::ElapsedRealtimeFlags;
                    using ::android::hardware::gnss::V2_0::GnssConstellationType;
                    using ::android::hardware::gnss::V2_0::GnssLocation;
                    void convertGnssLocation(Location &in, V1_0::GnssLocation &out)
                    {
                        memset(&out, 0, sizeof(V1_0::GnssLocation));
                        if (in.flags & LOCATION_HAS_LAT_LONG_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_LAT_LONG;
                            out.latitudeDegrees = in.latitude;
                            out.longitudeDegrees = in.longitude;
                        }
                        if (in.flags & LOCATION_HAS_ALTITUDE_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_ALTITUDE;
                            out.altitudeMeters = in.altitude;
                        }
                        if (in.flags & LOCATION_HAS_SPEED_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_SPEED;
                            out.speedMetersPerSec = in.speed;
                        }
                        if (in.flags & LOCATION_HAS_BEARING_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_BEARING;
                            out.bearingDegrees = in.bearing;
                        }
                        if (in.flags & LOCATION_HAS_ACCURACY_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_HORIZONTAL_ACCURACY;
                            out.horizontalAccuracyMeters = in.accuracy;
                        }
                        if (in.flags & LOCATION_HAS_VERTICAL_ACCURACY_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_VERTICAL_ACCURACY;
                            out.verticalAccuracyMeters = in.verticalAccuracy;
                        }
                        if (in.flags & LOCATION_HAS_SPEED_ACCURACY_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_SPEED_ACCURACY;
                            out.speedAccuracyMetersPerSecond = in.speedAccuracy;
                        }
                        if (in.flags & LOCATION_HAS_BEARING_ACCURACY_BIT)
                        {
                            out.gnssLocationFlags |= GnssLocationFlags::HAS_BEARING_ACCURACY;
                            out.bearingAccuracyDegrees = in.bearingAccuracy;
                        }
                        out.timestamp = static_cast<V1_0::GnssUtcTime>(in.timestamp);
                    }
                    bool getCurrentTime(struct timespec &currentTime, int64_t &sinceBootTimeNanos)
                    {
                        struct timespec sinceBootTime;
                        struct timespec sinceBootTimeTest;
                        bool clockGetTimeSuccess = false;
                        const uint32_t MAX_TIME_DELTA_VALUE_NANOS = 10000;
                        const uint32_t MAX_GET_TIME_COUNT = 20;
                        /* Attempt to get CLOCK_REALTIME and CLOCK_BOOTIME in succession without an interruption
                        or context switch (for up to MAX_GET_TIME_COUNT times) to avoid errors in the calculation */
                        for (uint32_t i = 0; i < MAX_GET_TIME_COUNT; i++)
                        {
                            if (clock_gettime(CLOCK_BOOTTIME, &sinceBootTime) != 0)
                            {
                                break;
                            };
                            if (clock_gettime(CLOCK_REALTIME, &currentTime) != 0)
                            {
                                break;
                            }
                            if (clock_gettime(CLOCK_BOOTTIME, &sinceBootTimeTest) != 0)
                            {
                                break;
                            };
                            sinceBootTimeNanos = sinceBootTime.tv_sec * 1000000000 + sinceBootTime.tv_nsec;
                            int64_t sinceBootTimeTestNanos =
                                sinceBootTimeTest.tv_sec * 1000000000 + sinceBootTimeTest.tv_nsec;
                            int64_t sinceBootTimeDeltaNanos = sinceBootTimeTestNanos - sinceBootTimeNanos;
                            /* sinceBootTime and sinceBootTimeTest should have a close value if there was no
                            interruption or context switch between clock_gettime for CLOCK_BOOTIME and
                            clock_gettime for CLOCK_REALTIME */
                            if (sinceBootTimeDeltaNanos < MAX_TIME_DELTA_VALUE_NANOS)
                            {
                                clockGetTimeSuccess = true;
                                break;
                            }
                            else
                            {
                                LOC_LOGd("Delta:%" PRIi64 "ns time too large, retry number #%u...",
                                         sinceBootTimeDeltaNanos, i + 1);
                            }
                        }
                        return clockGetTimeSuccess;
                    }
                    void convertGnssLocation(Location &in, V2_0::GnssLocation &out)
                    {
                        memset(&out, 0, sizeof(V2_0::GnssLocation));
                        convertGnssLocation(in, out.v1_0);
                        struct timespec currentTime;
                        int64_t sinceBootTimeNanos;
                        if (getCurrentTime(currentTime, sinceBootTimeNanos))
                        {
                            if (in.flags & LOCATION_HAS_ELAPSED_REAL_TIME)
                            {
                                uint64_t qtimerDiff = 0;
                                uint64_t qTimerTickCount = getQTimerTickCount();
                                if (qTimerTickCount >= in.elapsedRealTime)
                                {
                                    qtimerDiff = qTimerTickCount - in.elapsedRealTime;
                                }
                                LOC_LOGv("sinceBootTimeNanos:%" PRIi64 " in.elapsedRealTime=%" PRIi64 ""
                                         " qTimerTickCount=%" PRIi64 " qtimerDiff=%" PRIi64 "",
                                         sinceBootTimeNanos, in.elapsedRealTime, qTimerTickCount, qtimerDiff);
                                uint64_t qTimerDiffNanos = qTimerTicksToNanos(double(qtimerDiff));
                                /* If the time difference between Qtimer on modem side and Qtimer on AP side
                                   is greater than one second we assume this is a dual-SoC device such as
                                   Kona and will try to get Qtimer on modem side and on AP side and
                                   will adjust our difference accordingly */
                                if (qTimerDiffNanos > 1000000000)
                                {
                                    uint64_t qtimerDelta = getQTimerDeltaNanos();
                                    if (qTimerDiffNanos >= qtimerDelta)
                                    {
                                        qTimerDiffNanos -= qtimerDelta;
                                    }
                                }
                                if (sinceBootTimeNanos >= qTimerDiffNanos)
                                {
                                    out.elapsedRealtime.flags |= ElapsedRealtimeFlags::HAS_TIMESTAMP_NS;
                                    out.elapsedRealtime.timestampNs = sinceBootTimeNanos - qTimerDiffNanos;
                                    out.elapsedRealtime.flags |= ElapsedRealtimeFlags::HAS_TIME_UNCERTAINTY_NS;
                                    out.elapsedRealtime.timeUncertaintyNs = in.elapsedRealTimeUnc;
                                }
                            }
                            else
                            {
                                int64_t currentTimeNanos = currentTime.tv_sec * 1000000000 + currentTime.tv_nsec;
                                int64_t locationTimeNanos = in.timestamp * 1000000;
                                LOC_LOGv("sinceBootTimeNanos:%" PRIi64 " currentTimeNanos:%" PRIi64 ""
                                         " locationTimeNanos:%" PRIi64 "",
                                         sinceBootTimeNanos, currentTimeNanos, locationTimeNanos);
                                if (currentTimeNanos >= locationTimeNanos)
                                {
                                    int64_t ageTimeNanos = currentTimeNanos - locationTimeNanos;
                                    LOC_LOGv("ageTimeNanos:%" PRIi64 ")", ageTimeNanos);
                                    // the max trusted propagation time 100ms for ageTimeNanos to avoid user setting
                                    // wrong time, it will affect elapsedRealtimeNanos
                                    if (ageTimeNanos <= 100000000)
                                    {
                                        out.elapsedRealtime.flags |= ElapsedRealtimeFlags::HAS_TIMESTAMP_NS;
                                        out.elapsedRealtime.timestampNs = sinceBootTimeNanos - ageTimeNanos;
                                        out.elapsedRealtime.flags |= ElapsedRealtimeFlags::HAS_TIME_UNCERTAINTY_NS;
                                        // time uncertainty is the max value between abs(AP_UTC - MP_UTC) and 100ms, to
                                        // verify if user change the sys time
                                        out.elapsedRealtime.timeUncertaintyNs =
                                            std::max(ageTimeNanos, (int64_t)100000000);
                                    }
                                }
                            }
                        }
                        LOC_LOGv("out.elapsedRealtime.timestampNs=%" PRIi64 ""
                                 " out.elapsedRealtime.timeUncertaintyNs=%" PRIi64 ""
                                 " out.elapsedRealtime.flags=0x%X",
                                 out.elapsedRealtime.timestampNs,
                                 out.elapsedRealtime.timeUncertaintyNs, out.elapsedRealtime.flags);
                    }
                    void convertGnssLocation(const V1_0::GnssLocation &in, Location &out)
                    {
                        memset(&out, 0, sizeof(out));
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_LAT_LONG)
                        {
                            out.flags |= LOCATION_HAS_LAT_LONG_BIT;
                            out.latitude = in.latitudeDegrees;
                            out.longitude = in.longitudeDegrees;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_ALTITUDE)
                        {
                            out.flags |= LOCATION_HAS_ALTITUDE_BIT;
                            out.altitude = in.altitudeMeters;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_SPEED)
                        {
                            out.flags |= LOCATION_HAS_SPEED_BIT;
                            out.speed = in.speedMetersPerSec;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_BEARING)
                        {
                            out.flags |= LOCATION_HAS_BEARING_BIT;
                            out.bearing = in.bearingDegrees;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_HORIZONTAL_ACCURACY)
                        {
                            out.flags |= LOCATION_HAS_ACCURACY_BIT;
                            out.accuracy = in.horizontalAccuracyMeters;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_VERTICAL_ACCURACY)
                        {
                            out.flags |= LOCATION_HAS_VERTICAL_ACCURACY_BIT;
                            out.verticalAccuracy = in.verticalAccuracyMeters;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_SPEED_ACCURACY)
                        {
                            out.flags |= LOCATION_HAS_SPEED_ACCURACY_BIT;
                            out.speedAccuracy = in.speedAccuracyMetersPerSecond;
                        }
                        if (in.gnssLocationFlags & GnssLocationFlags::HAS_BEARING_ACCURACY)
                        {
                            out.flags |= LOCATION_HAS_BEARING_ACCURACY_BIT;
                            out.bearingAccuracy = in.bearingAccuracyDegrees;
                        }
                        out.timestamp = static_cast<uint64_t>(in.timestamp);
                    }
                    void convertGnssLocation(const V2_0::GnssLocation &in, Location &out)
                    {
                        memset(&out, 0, sizeof(out));
                        convertGnssLocation(in.v1_0, out);
                    }
                    void convertGnssConstellationType(GnssSvType &in, V1_0::GnssConstellationType &out)
                    {
                        switch (in)
                        {
                        case GNSS_SV_TYPE_GPS:
                            out = V1_0::GnssConstellationType::GPS;
                            break;
                        case GNSS_SV_TYPE_SBAS:
                            out = V1_0::GnssConstellationType::SBAS;
                            break;
                        case GNSS_SV_TYPE_GLONASS:
                            out = V1_0::GnssConstellationType::GLONASS;
                            break;
                        case GNSS_SV_TYPE_QZSS:
                            out = V1_0::GnssConstellationType::QZSS;
                            break;
                        case GNSS_SV_TYPE_BEIDOU:
                            out = V1_0::GnssConstellationType::BEIDOU;
                            break;
                        case GNSS_SV_TYPE_GALILEO:
                            out = V1_0::GnssConstellationType::GALILEO;
                            break;
                        case GNSS_SV_TYPE_UNKNOWN:
                        default:
                            out = V1_0::GnssConstellationType::UNKNOWN;
                            break;
                        }
                    }
                    void convertGnssConstellationType(GnssSvType &in, V2_0::GnssConstellationType &out)
                    {
                        switch (in)
                        {
                        case GNSS_SV_TYPE_GPS:
                            out = V2_0::GnssConstellationType::GPS;
                            break;
                        case GNSS_SV_TYPE_SBAS:
                            out = V2_0::GnssConstellationType::SBAS;
                            break;
                        case GNSS_SV_TYPE_GLONASS:
                            out = V2_0::GnssConstellationType::GLONASS;
                            break;
                        case GNSS_SV_TYPE_QZSS:
                            out = V2_0::GnssConstellationType::QZSS;
                            break;
                        case GNSS_SV_TYPE_BEIDOU:
                            out = V2_0::GnssConstellationType::BEIDOU;
                            break;
                        case GNSS_SV_TYPE_GALILEO:
                            out = V2_0::GnssConstellationType::GALILEO;
                            break;
                        case GNSS_SV_TYPE_NAVIC:
                            out = V2_0::GnssConstellationType::IRNSS;
                            break;
                        case GNSS_SV_TYPE_UNKNOWN:
                        default:
                            out = V2_0::GnssConstellationType::UNKNOWN;
                            break;
                        }
                    }
                    void convertGnssSvid(GnssSv &in, int16_t &out)
                    {
                        switch (in.type)
                        {
                        case GNSS_SV_TYPE_GPS:
                            out = in.svId;
                            break;
                        case GNSS_SV_TYPE_SBAS:
                            out = in.svId;
                            break;
                        case GNSS_SV_TYPE_GLONASS:
                            out = in.svId - GLO_SV_PRN_MIN + 1;
                            break;
                        case GNSS_SV_TYPE_QZSS:
                            out = in.svId;
                            break;
                        case GNSS_SV_TYPE_BEIDOU:
                            out = in.svId - BDS_SV_PRN_MIN + 1;
                            break;
                        case GNSS_SV_TYPE_GALILEO:
                            out = in.svId - GAL_SV_PRN_MIN + 1;
                            break;
                        case GNSS_SV_TYPE_NAVIC:
                            /*Android doesn't define Navic svid range yet, use Naviv svid [1, 14] now
                              will update this once Android give Navic svid definiitons */
                            out = in.svId - NAVIC_SV_PRN_MIN + 1;
                            break;
                        default:
                            out = in.svId;
                            break;
                        }
                    }
                    void convertGnssSvid(GnssMeasurementsData &in, int16_t &out)
                    {
                        switch (in.svType)
                        {
                        case GNSS_SV_TYPE_GPS:
                            out = in.svId;
                            break;
                        case GNSS_SV_TYPE_SBAS:
                            out = in.svId;
                            break;
                        case GNSS_SV_TYPE_GLONASS:
                            if (in.svId != 255)
                            { // OSN is known
                                out = in.svId - GLO_SV_PRN_MIN + 1;
                            }
                            else
                            { // OSN is not known, report FCN
                                out = in.gloFrequency + 92;
                            }
                            break;
                        case GNSS_SV_TYPE_QZSS:
                            out = in.svId;
                            break;
                        case GNSS_SV_TYPE_BEIDOU:
                            out = in.svId - BDS_SV_PRN_MIN + 1;
                            break;
                        case GNSS_SV_TYPE_GALILEO:
                            out = in.svId - GAL_SV_PRN_MIN + 1;
                            break;
                        case GNSS_SV_TYPE_NAVIC:
                            /*Android doesn't define Navic svid range yet, use Naviv svid [1, 14] now
                              will update this once Android give Navic svid definiitons */
                            out = in.svId - NAVIC_SV_PRN_MIN + 1;
                            break;
                        default:
                            out = in.svId;
                            break;
                        }
                    }
                    void convertGnssEphemerisType(GnssEphemerisType &in, GnssDebug::SatelliteEphemerisType &out)
                    {
                        switch (in)
                        {
                        case GNSS_EPH_TYPE_EPHEMERIS:
                            out = GnssDebug::SatelliteEphemerisType::EPHEMERIS;
                            break;
                        case GNSS_EPH_TYPE_ALMANAC:
                            out = GnssDebug::SatelliteEphemerisType::ALMANAC_ONLY;
                            break;
                        case GNSS_EPH_TYPE_UNKNOWN:
                        default:
                            out = GnssDebug::SatelliteEphemerisType::NOT_AVAILABLE;
                            break;
                        }
                    }
                    void convertGnssEphemerisSource(GnssEphemerisSource &in, GnssDebug::SatelliteEphemerisSource &out)
                    {
                        switch (in)
                        {
                        case GNSS_EPH_SOURCE_DEMODULATED:
                            out = GnssDebug::SatelliteEphemerisSource::DEMODULATED;
                            break;
                        case GNSS_EPH_SOURCE_SUPL_PROVIDED:
                            out = GnssDebug::SatelliteEphemerisSource::SUPL_PROVIDED;
                            break;
                        case GNSS_EPH_SOURCE_OTHER_SERVER_PROVIDED:
                            out = GnssDebug::SatelliteEphemerisSource::OTHER_SERVER_PROVIDED;
                            break;
                        case GNSS_EPH_SOURCE_LOCAL:
                        case GNSS_EPH_SOURCE_UNKNOWN:
                        default:
                            out = GnssDebug::SatelliteEphemerisSource::OTHER;
                            break;
                        }
                    }
                    void convertGnssEphemerisHealth(GnssEphemerisHealth &in, GnssDebug::SatelliteEphemerisHealth &out)
                    {
                        switch (in)
                        {
                        case GNSS_EPH_HEALTH_GOOD:
                            out = GnssDebug::SatelliteEphemerisHealth::GOOD;
                            break;
                        case GNSS_EPH_HEALTH_BAD:
                            out = GnssDebug::SatelliteEphemerisHealth::BAD;
                            break;
                        case GNSS_EPH_HEALTH_UNKNOWN:
                        default:
                            out = GnssDebug::SatelliteEphemerisHealth::UNKNOWN;
                            break;
                        }
                    }
                    void convertSingleSatCorrections(const SingleSatCorrection &in, GnssSingleSatCorrection &out)
                    {
                        out.flags = GNSS_MEAS_CORR_UNKNOWN_BIT;
                        if (in.singleSatCorrectionFlags & (GnssSingleSatCorrectionFlags::HAS_SAT_IS_LOS_PROBABILITY))
                        {
                            out.flags |= GNSS_MEAS_CORR_HAS_SAT_IS_LOS_PROBABILITY_BIT;
                        }
                        if (in.singleSatCorrectionFlags & (GnssSingleSatCorrectionFlags::HAS_EXCESS_PATH_LENGTH))
                        {
                            out.flags |= GNSS_MEAS_CORR_HAS_EXCESS_PATH_LENGTH_BIT;
                        }
                        if (in.singleSatCorrectionFlags & (GnssSingleSatCorrectionFlags::HAS_EXCESS_PATH_LENGTH_UNC))
                        {
                            out.flags |= GNSS_MEAS_CORR_HAS_EXCESS_PATH_LENGTH_UNC_BIT;
                        }
                        if (in.singleSatCorrectionFlags & (GnssSingleSatCorrectionFlags::HAS_REFLECTING_PLANE))
                        {
                            out.flags |= GNSS_MEAS_CORR_HAS_REFLECTING_PLANE_BIT;
                        }
                        switch (in.constellation)
                        {
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::GPS):
                            out.svType = GNSS_SV_TYPE_GPS;
                            break;
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::SBAS):
                            out.svType = GNSS_SV_TYPE_SBAS;
                            break;
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::GLONASS):
                            out.svType = GNSS_SV_TYPE_GLONASS;
                            break;
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::QZSS):
                            out.svType = GNSS_SV_TYPE_QZSS;
                            break;
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::BEIDOU):
                            out.svType = GNSS_SV_TYPE_BEIDOU;
                            break;
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::GALILEO):
                            out.svType = GNSS_SV_TYPE_GALILEO;
                            break;
                        case (::android::hardware::gnss::V1_0::GnssConstellationType::UNKNOWN):
                        default:
                            out.svType = GNSS_SV_TYPE_UNKNOWN;
                            break;
                        }
                        out.svId = in.svid;
                        out.carrierFrequencyHz = in.carrierFrequencyHz;
                        out.probSatIsLos = in.probSatIsLos;
                        out.excessPathLengthMeters = in.excessPathLengthMeters;
                        out.excessPathLengthUncertaintyMeters = in.excessPathLengthUncertaintyMeters;
                        out.reflectingPlane.latitudeDegrees = in.reflectingPlane.latitudeDegrees;
                        out.reflectingPlane.longitudeDegrees = in.reflectingPlane.longitudeDegrees;
                        out.reflectingPlane.altitudeMeters = in.reflectingPlane.altitudeMeters;
                        out.reflectingPlane.azimuthDegrees = in.reflectingPlane.azimuthDegrees;
                    }
                    void convertMeasurementCorrections(const MeasurementCorrectionsV1_0 &in,
                                                       GnssMeasurementCorrections &out)
                    {
                        memset(&out, 0, sizeof(GnssMeasurementCorrections));
                        out.latitudeDegrees = in.latitudeDegrees;
                        out.longitudeDegrees = in.longitudeDegrees;
                        out.altitudeMeters = in.altitudeMeters;
                        out.horizontalPositionUncertaintyMeters = in.horizontalPositionUncertaintyMeters;
                        out.verticalPositionUncertaintyMeters = in.verticalPositionUncertaintyMeters;
                        out.toaGpsNanosecondsOfWeek = in.toaGpsNanosecondsOfWeek;
                        for (int i = 0; i < in.satCorrections.size(); i++)
                        {
                            GnssSingleSatCorrection gnssSingleSatCorrection = {};
                            convertSingleSatCorrections(in.satCorrections[i], gnssSingleSatCorrection);
                            out.satCorrections.push_back(gnssSingleSatCorrection);
                        }
                    }
                } // namespace implementation
            }     // namespace V2_1
        }         // namespace gnss
    }             // namespace hardware
} // namespace android
