/*
 * Copyright (c) 2019-2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */

#define LOG_TAG "LocSvc_MeasurementCorrectionsInterface"

#include "MeasurementCorrections.h"

#include <log_util.h>

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
using ::android::hardware::gnss::V1_0::GnssLocation;

MeasurementCorrections::MeasurementCorrections() {}

MeasurementCorrections::~MeasurementCorrections() {}

Return<bool> MeasurementCorrections::setCorrections(
    const ::android::hardware::gnss::measurement_corrections::V1_0::
        MeasurementCorrections& /*corrections*/) {
  return true;
}

Return<bool> MeasurementCorrections::setCallback(
    const sp<V1_0::IMeasurementCorrectionsCallback>& /*callback*/) {
  return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace measurement_corrections
}  // namespace gnss
}  // namespace hardware
}  // namespace android
