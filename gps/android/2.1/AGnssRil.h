/*
 * Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ANDROID_HARDWARE_GNSS_V2_0_AGNSSRIL_H_
#define ANDROID_HARDWARE_GNSS_V2_0_AGNSSRIL_H_
#include <android/hardware/gnss/2.0/IAGnssRil.h>
#include <hidl/Status.h>
#include <location_interface.h>
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
                    struct Gnss;
                    /*
                     * Extended interface for AGNSS RIL support. An Assisted GNSS Radio Interface Layer interface
                     * allows the GNSS chipset to request radio interface layer information from Android platform.
                     * Examples of such information are reference location, unique subscriber ID, phone number string
                     * and network availability changes. Also contains wrapper methods to allow methods from
                     * IAGnssiRilCallback interface to be passed into the conventional implementation of the GNSS HAL.
                     */
                    struct AGnssRil : public V2_0::IAGnssRil
                    {
                        AGnssRil(Gnss *gnss);
                        ~AGnssRil();
                        /*
                         * Methods from ::android::hardware::gnss::V1_0::IAGnssRil follow.
                         * These declarations were generated from IAGnssRil.hal.
                         */
                        Return<void> setCallback(const sp<V1_0::IAGnssRilCallback> & /*callback*/) override
                        {
                            return Void();
                        }
                        Return<void> setRefLocation(const V1_0::IAGnssRil::AGnssRefLocation & /*agnssReflocation*/) override
                        {
                            return Void();
                        }
                        Return<bool> setSetId(V1_0::IAGnssRil::SetIDType /*type*/, const hidl_string & /*setid*/) override
                        {
                            return false;
                        }
                        Return<bool> updateNetworkAvailability(bool /*available*/,
                                                               const hidl_string & /*apn*/) override
                        {
                            return false;
                        }
                        Return<bool> updateNetworkState(bool connected, V1_0::IAGnssRil::NetworkType type, bool roaming) override;
                        // Methods from ::android::hardware::gnss::V2_0::IAGnssRil follow
                        Return<bool> updateNetworkState_2_0(const V2_0::IAGnssRil::NetworkAttributes &attributes) override;
                    private:
                        Gnss *mGnss = nullptr;
                    };
                } // namespace implementation
            }     // namespace V2_1
        }         // namespace gnss
    }             // namespace hardware
} // namespace android
#endif // ANDROID_HARDWARE_GNSS_V2_0_AGNSSRIL_H_