/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */

/* Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_1_GNSSCONFIGURATION_H
#define ANDROID_HARDWARE_GNSS_V1_1_GNSSCONFIGURATION_H

#include <android/hardware/gnss/1.1/IGnssConfiguration.h>
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
using ::android::hardware::gnss::V1_1::IGnssConfiguration;

/*
 * Interface for passing GNSS configuration info from platform to HAL.
 */
struct Gnss;
struct GnssConfiguration : public IGnssConfiguration {
  GnssConfiguration(Gnss* gnss);
  ~GnssConfiguration() = default;

  /*
   * Methods from ::android::hardware::gnss::V1_0::IGnssConfiguration follow.
   * These declarations were generated from IGnssConfiguration.hal.
   */
  Return<bool> setSuplVersion(uint32_t version) override;
  Return<bool> setSuplMode(uint8_t mode) override;
  Return<bool> setSuplEs(bool enabled) override;
  Return<bool> setLppProfile(uint8_t lppProfileMask) override;
  Return<bool> setGlonassPositioningProtocol(uint8_t protocol) override;
  Return<bool> setEmergencySuplPdn(bool enable) override;
  Return<bool> setGpsLock(uint8_t lock) override;

  // Methods from ::android::hardware::gnss::V1_1::IGnssConfiguration follow.
  Return<bool> setBlacklist(
      const hidl_vec<GnssConfiguration::BlacklistedSource>& blacklist) override;

 private:
  Gnss* mGnss = nullptr;
  bool setBlacklistedSource(
      GnssSvIdSource& copyToSource,
      const GnssConfiguration::BlacklistedSource& copyFromSource);
};

}  // namespace implementation
}  // namespace V1_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_1_GNSSCONFIGURATION_H
