/* Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef MEASUREMENT_API_CLINET_H
#define MEASUREMENT_API_CLINET_H

#include <LocationAPIClientBase.h>
#include <android/hardware/gnss/1.1/IGnssMeasurement.h>
#include <android/hardware/gnss/1.1/IGnssMeasurementCallback.h>
#include <gps_extended_c.h>
#include <hidl/Status.h>

#include <mutex>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_1 {
namespace implementation {

using ::android::sp;

class MeasurementAPIClient : public LocationAPIClientBase {
 public:
  MeasurementAPIClient();
  MeasurementAPIClient(const MeasurementAPIClient&) = delete;
  MeasurementAPIClient& operator=(const MeasurementAPIClient&) = delete;

  // for GpsMeasurementInterface
  Return<V1_0::IGnssMeasurement::GnssMeasurementStatus> measurementSetCallback(
      const sp<V1_0::IGnssMeasurementCallback>& callback);
  Return<V1_0::IGnssMeasurement::GnssMeasurementStatus>
  measurementSetCallback_1_1(
      const sp<IGnssMeasurementCallback>& callback,
      GnssPowerMode powerMode = GNSS_POWER_MODE_INVALID,
      uint32_t timeBetweenMeasurement = GPS_DEFAULT_FIX_INTERVAL_MS);
  void measurementClose();
  Return<IGnssMeasurement::GnssMeasurementStatus> startTracking(
      GnssPowerMode powerMode = GNSS_POWER_MODE_INVALID,
      uint32_t timeBetweenMeasurement = GPS_DEFAULT_FIX_INTERVAL_MS);

  // callbacks we are interested in
  void onGnssMeasurementsCb(
      GnssMeasurementsNotification gnssMeasurementsNotification) final;

 private:
  virtual ~MeasurementAPIClient();

  std::mutex mMutex;
  sp<V1_0::IGnssMeasurementCallback> mGnssMeasurementCbIface;
  sp<IGnssMeasurementCallback> mGnssMeasurementCbIface_1_1;

  bool mTracking;
};

}  // namespace implementation
}  // namespace V1_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif  // MEASUREMENT_API_CLINET_H
