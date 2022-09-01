/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_1_GNSSMEASUREMENT_H
#define ANDROID_HARDWARE_GNSS_V1_1_GNSSMEASUREMENT_H

#include <android/hardware/gnss/1.1/IGnssMeasurement.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_1 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::gnss::V1_1::IGnssMeasurement;
using ::android::hardware::gnss::V1_1::IGnssMeasurementCallback;

class MeasurementAPIClient;
struct GnssMeasurement : public IGnssMeasurement {
  GnssMeasurement();
  ~GnssMeasurement();

  /*
   * Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.
   * These declarations were generated from IGnssMeasurement.hal.
   */
  Return<GnssMeasurement::GnssMeasurementStatus> setCallback(
      const sp<V1_0::IGnssMeasurementCallback>& callback) override;
  Return<void> close() override;

  // Methods from ::android::hardware::gnss::V1_1::IGnssMeasurement follow.
  Return<GnssMeasurement::GnssMeasurementStatus> setCallback_1_1(
      const sp<IGnssMeasurementCallback>& callback,
      bool enableFullTracking) override;

 private:
  struct GnssMeasurementDeathRecipient : hidl_death_recipient {
    GnssMeasurementDeathRecipient(sp<GnssMeasurement> gnssMeasurement)
        : mGnssMeasurement(gnssMeasurement) {}
    ~GnssMeasurementDeathRecipient() = default;
    virtual void serviceDied(uint64_t cookie, const wp<IBase>& who) override;
    sp<GnssMeasurement> mGnssMeasurement;
  };

 private:
  sp<GnssMeasurementDeathRecipient> mGnssMeasurementDeathRecipient = nullptr;
  sp<V1_0::IGnssMeasurementCallback> mGnssMeasurementCbIface = nullptr;
  sp<IGnssMeasurementCallback> mGnssMeasurementCbIface_1_1 = nullptr;
  MeasurementAPIClient* mApi;
};

}  // namespace implementation
}  // namespace V1_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_1_GNSSMEASUREMENT_H
