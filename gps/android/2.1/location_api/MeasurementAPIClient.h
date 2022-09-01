/* Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef MEASUREMENT_API_CLINET_H
#define MEASUREMENT_API_CLINET_H

#include <android/hardware/gnss/2.1/IGnssMeasurement.h>

#include <mutex>
//#include <android/hardware/gnss/1.1/IGnssMeasurementCallback.h>
#include <LocationAPIClientBase.h>
#include <android/hardware/gnss/2.1/IGnssMeasurementCallback.h>
#include <gps_extended_c.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V2_1 {
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
      const sp<V1_1::IGnssMeasurementCallback>& callback,
      GnssPowerMode powerMode = GNSS_POWER_MODE_INVALID,
      uint32_t timeBetweenMeasurement = GPS_DEFAULT_FIX_INTERVAL_MS);
  Return<V1_0::IGnssMeasurement::GnssMeasurementStatus>
  measurementSetCallback_2_0(
      const sp<V2_0::IGnssMeasurementCallback>& callback,
      GnssPowerMode powerMode = GNSS_POWER_MODE_INVALID,
      uint32_t timeBetweenMeasurement = GPS_DEFAULT_FIX_INTERVAL_MS);
  Return<V1_0::IGnssMeasurement::GnssMeasurementStatus>
  measurementSetCallback_2_1(
      const sp<V2_1::IGnssMeasurementCallback>& callback,
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
  sp<V1_1::IGnssMeasurementCallback> mGnssMeasurementCbIface_1_1;
  sp<V2_0::IGnssMeasurementCallback> mGnssMeasurementCbIface_2_0;
  sp<V2_1::IGnssMeasurementCallback> mGnssMeasurementCbIface_2_1;
  bool mTracking;
  void clearInterfaces();
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif  // MEASUREMENT_API_CLINET_H
