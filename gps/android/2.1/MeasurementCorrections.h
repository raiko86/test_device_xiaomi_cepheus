/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_1_MeasurementCorrections_H
#define ANDROID_HARDWARE_GNSS_V1_1_MeasurementCorrections_H

#include <LocationUtil.h>
#include <android/hardware/gnss/measurement_corrections/1.0/IMeasurementCorrectionsCallback.h>
#include <android/hardware/gnss/measurement_corrections/1.1/IMeasurementCorrections.h>
#include <gps_extended_c.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <location_interface.h>

#include "Gnss.h"

namespace android {
namespace hardware {
namespace gnss {
namespace measurement_corrections {
namespace V1_1 {
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
using ::android::hardware::gnss::V2_1::implementation::Gnss;
using MeasurementCorrectionsV1_0 = ::android::hardware::gnss::
    measurement_corrections::V1_0::MeasurementCorrections;
using MeasurementCorrectionsV1_1 = ::android::hardware::gnss::
    measurement_corrections::V1_1::MeasurementCorrections;

struct MeasurementCorrections : public V1_1::IMeasurementCorrections {
  MeasurementCorrections(Gnss* gnss);
  ~MeasurementCorrections();

  Return<bool> setCorrections(
      const MeasurementCorrectionsV1_0& corrections) override;

  // Methods from
  // ::android::hardware::gnss::measurement_corrections::V1_1::IMeasurementCorrections
  // follow.
  Return<bool> setCorrections_1_1(
      const MeasurementCorrectionsV1_1& corrections);

  Return<bool> setCallback(
      const sp<IMeasurementCorrectionsCallback>& callback) override;

  void setCapabilitiesCb(
      GnssMeasurementCorrectionsCapabilitiesMask capabilities);

  /* Data call setup callback passed down to GNSS HAL implementation */
  static void measCorrSetCapabilitiesCb(
      GnssMeasurementCorrectionsCapabilitiesMask capabilities);

 private:
  struct GnssMeasurementCorrectionsDeathRecipient : hidl_death_recipient {
    GnssMeasurementCorrectionsDeathRecipient(
        sp<MeasurementCorrections> gnssMeasurementCorrections)
        : mGnssMeasurementCorrections(gnssMeasurementCorrections) {}
    ~GnssMeasurementCorrectionsDeathRecipient() = default;
    virtual void serviceDied(uint64_t cookie, const wp<IBase>& who) override;
    sp<MeasurementCorrections> mGnssMeasurementCorrections;
  };
  Gnss* mGnss = nullptr;
  sp<GnssMeasurementCorrectionsDeathRecipient>
      mGnssMeasurementCorrectionsDeathRecipient = nullptr;
  sp<IMeasurementCorrectionsCallback> mMeasurementCorrectionsCbIface = nullptr;
};

}  // namespace implementation
}  // namespace V1_1
}  // namespace measurement_corrections
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_1_MeasurementCorrections_H
