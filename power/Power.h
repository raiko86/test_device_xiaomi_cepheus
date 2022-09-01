/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/power/1.3/IPower.h>
#include <hidl/Status.h>
#include <utils/Log.h>

namespace android {
namespace hardware {
namespace power {
namespace V1_3 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::Status;
using PowerHint_1_0 = ::android::hardware::power::V1_0::PowerHint;
using PowerHint_1_2 = ::android::hardware::power::V1_2::PowerHint;
using PowerHint_1_3 = ::android::hardware::power::V1_3::PowerHint;

struct Power : public IPower {
  // Methods from V1_0::IPower follow.
  Return<void> setInteractive(bool interactive) override;
  Return<void> powerHint(PowerHint_1_0 hint, int32_t data) override;
  Return<void> setFeature(V1_0::Feature feature, bool activate) override;
  Return<void> getPlatformLowPowerStats(
      getPlatformLowPowerStats_cb _hidl_cb) override;

  // Methods from V1_1::IPower follow.
  Return<void> getSubsystemLowPowerStats(
      getSubsystemLowPowerStats_cb _hidl_cb) override;
  Return<void> powerHintAsync(PowerHint_1_0 hint, int32_t data) override;

  // Methods from V1_2::IPower follow.
  Return<void> powerHintAsync_1_2(PowerHint_1_2 hint, int32_t data) override;

  // Methods from V1_3::IPower follow.
  Return<void> powerHintAsync_1_3(PowerHint_1_3 hint, int32_t data) override;
};

}  // namespace implementation
}  // namespace V1_3
}  // namespace power
}  // namespace hardware
}  // namespace android
