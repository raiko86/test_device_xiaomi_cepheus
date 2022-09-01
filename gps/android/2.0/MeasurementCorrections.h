/*
 * Copyright (c) 2019-2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_0_MeasurementCorrections_H
#define ANDROID_HARDWARE_GNSS_V1_0_MeasurementCorrections_H

#include <android/hardware/gnss/measurement_corrections/1.0/IMeasurementCorrections.h>
#include <android/hardware/gnss/measurement_corrections/1.0/IMeasurementCorrectionsCallback.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <location_interface.h>

namespace android {
namespace hardware {
namespace gnss {
namespace measurement_corrections {
namespace V1_0 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::gnss::measurement_corrections::V1_0::
    IMeasurementCorrectionsCallback;
using ::android::hardware::gnss::V1_0::GnssLocation;

struct MeasurementCorrections : public IMeasurementCorrections {
  MeasurementCorrections();
  ~MeasurementCorrections();

  // Methods from
  // ::android::hardware::gnss::measurement_corrections::V1_0::IMeasurementCorrections
  // follow.
  Return<bool> setCorrections(
      const ::android::hardware::gnss::measurement_corrections::V1_0::
          MeasurementCorrections& corrections) override;

  Return<bool> setCallback(
      const sp<IMeasurementCorrectionsCallback>& callback) override;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace measurement_corrections
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_MeasurementCorrections_H
