/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "LocSvc_GnssMeasurementInterface"

#include "GnssMeasurement.h"

#include <MeasurementAPIClient.h>
#include <log_util.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_1 {
namespace implementation {

void GnssMeasurement::GnssMeasurementDeathRecipient::serviceDied(
    uint64_t cookie, const wp<IBase>& who) {
  LOC_LOGE("%s] service died. cookie: %llu, who: %p", __FUNCTION__,
           static_cast<unsigned long long>(cookie), &who);
  if (mGnssMeasurement != nullptr) {
    mGnssMeasurement->close();
  }
}

GnssMeasurement::GnssMeasurement() {
  mGnssMeasurementDeathRecipient = new GnssMeasurementDeathRecipient(this);
  mApi = new MeasurementAPIClient();
}

GnssMeasurement::~GnssMeasurement() {
  if (mApi) {
    mApi->destroy();
    mApi = nullptr;
  }
}

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.

Return<IGnssMeasurement::GnssMeasurementStatus> GnssMeasurement::setCallback(
    const sp<V1_0::IGnssMeasurementCallback>& callback) {
  Return<IGnssMeasurement::GnssMeasurementStatus> ret =
      IGnssMeasurement::GnssMeasurementStatus::ERROR_GENERIC;
  if (mGnssMeasurementCbIface != nullptr) {
    LOC_LOGE("%s]: GnssMeasurementCallback is already set", __FUNCTION__);
    return IGnssMeasurement::GnssMeasurementStatus::ERROR_ALREADY_INIT;
  }

  if (callback == nullptr) {
    LOC_LOGE("%s]: callback is nullptr", __FUNCTION__);
    return ret;
  }
  if (mApi == nullptr) {
    LOC_LOGE("%s]: mApi is nullptr", __FUNCTION__);
    return ret;
  }

  mGnssMeasurementCbIface = callback;
  mGnssMeasurementCbIface->linkToDeath(mGnssMeasurementDeathRecipient, 0);

  return mApi->measurementSetCallback(callback);
}

Return<void> GnssMeasurement::close() {
  if (mApi == nullptr) {
    LOC_LOGE("%s]: mApi is nullptr", __FUNCTION__);
    return Void();
  }

  if (mGnssMeasurementCbIface != nullptr) {
    mGnssMeasurementCbIface->unlinkToDeath(mGnssMeasurementDeathRecipient);
    mGnssMeasurementCbIface = nullptr;
  }
  if (mGnssMeasurementCbIface_1_1 != nullptr) {
    mGnssMeasurementCbIface_1_1->unlinkToDeath(mGnssMeasurementDeathRecipient);
    mGnssMeasurementCbIface_1_1 = nullptr;
  }
  mApi->measurementClose();

  return Void();
}

// Methods from ::android::hardware::gnss::V1_1::IGnssMeasurement follow.
Return<GnssMeasurement::GnssMeasurementStatus> GnssMeasurement::setCallback_1_1(
    const sp<IGnssMeasurementCallback>& callback, bool enableFullTracking) {
  Return<IGnssMeasurement::GnssMeasurementStatus> ret =
      IGnssMeasurement::GnssMeasurementStatus::ERROR_GENERIC;
  if (mGnssMeasurementCbIface_1_1 != nullptr) {
    LOC_LOGE("%s]: GnssMeasurementCallback is already set", __FUNCTION__);
    return IGnssMeasurement::GnssMeasurementStatus::ERROR_ALREADY_INIT;
  }

  if (callback == nullptr) {
    LOC_LOGE("%s]: callback is nullptr", __FUNCTION__);
    return ret;
  }
  if (nullptr == mApi) {
    LOC_LOGE("%s]: mApi is nullptr", __FUNCTION__);
    return ret;
  }

  mGnssMeasurementCbIface_1_1 = callback;
  mGnssMeasurementCbIface_1_1->linkToDeath(mGnssMeasurementDeathRecipient, 0);

  GnssPowerMode powerMode =
      enableFullTracking ? GNSS_POWER_MODE_M1 : GNSS_POWER_MODE_M2;

  return mApi->measurementSetCallback_1_1(callback, powerMode);
}

}  // namespace implementation
}  // namespace V1_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android
