/*
 * Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ANDROID_HARDWARE_GNSS_V2_0_GNSSGEOFENCING_H
#define ANDROID_HARDWARE_GNSS_V2_0_GNSSGEOFENCING_H
#include <android/hardware/gnss/1.0/IGnssGeofencing.h>
#include <hidl/Status.h>
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
                    using ::android::sp;
                    using ::android::hardware::hidl_string;
                    using ::android::hardware::hidl_vec;
                    using ::android::hardware::Return;
                    using ::android::hardware::Void;
                    using ::android::hardware::gnss::V1_0::IGnssGeofenceCallback;
                    using ::android::hardware::gnss::V1_0::IGnssGeofencing;
                    class GeofenceAPIClient;
                    struct GnssGeofencing : public IGnssGeofencing
                    {
                        GnssGeofencing();
                        ~GnssGeofencing();
                        /*
                         * Methods from ::android::hardware::gnss::V1_0::IGnssGeofencing follow.
                         * These declarations were generated from IGnssGeofencing.hal.
                         */
                        Return<void> setCallback(const sp<IGnssGeofenceCallback> &callback) override;
                        Return<void> addGeofence(int32_t geofenceId,
                                                 double latitudeDegrees,
                                                 double longitudeDegrees,
                                                 double radiusMeters,
                                                 IGnssGeofenceCallback::GeofenceTransition lastTransition,
                                                 int32_t monitorTransitions,
                                                 uint32_t notificationResponsivenessMs,
                                                 uint32_t unknownTimerMs) override;
                        Return<void> pauseGeofence(int32_t geofenceId) override;
                        Return<void> resumeGeofence(int32_t geofenceId, int32_t monitorTransitions) override;
                        Return<void> removeGeofence(int32_t geofenceId) override;
                    private:
                        // This method is not part of the IGnss base class.
                        // It is called by GnssGeofencingDeathRecipient to remove all geofences added so far.
                        Return<void> removeAllGeofences();
                    private:
                        struct GnssGeofencingDeathRecipient : hidl_death_recipient
                        {
                            GnssGeofencingDeathRecipient(sp<GnssGeofencing> gnssGeofencing) : mGnssGeofencing(gnssGeofencing)
                            {
                            }
                            ~GnssGeofencingDeathRecipient() = default;
                            virtual void serviceDied(uint64_t cookie, const wp<IBase> &who) override;
                            sp<GnssGeofencing> mGnssGeofencing;
                        };
                    private:
                        sp<GnssGeofencingDeathRecipient> mGnssGeofencingDeathRecipient = nullptr;
                        sp<IGnssGeofenceCallback> mGnssGeofencingCbIface = nullptr;
                        GeofenceAPIClient *mApi = nullptr;
                    };
                } // namespace implementation
            }     // namespace V2_1
        }         // namespace gnss
    }             // namespace hardware
} // namespace android
#endif // ANDROID_HARDWARE_GNSS_V2_0_GNSSGEOFENCING_H