/*
 * Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ANDROID_HARDWARE_GNSS_V2_0_AGNSS_H
#define ANDROID_HARDWARE_GNSS_V2_0_AGNSS_H

#include <android/hardware/gnss/2.0/IAGnss.h>
#include <gps_extended_c.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

struct Gnss;
struct AGnss : public V2_0::IAGnss {
  AGnss(Gnss* gnss);
  ~AGnss();
  /*
   * Methods from ::android::hardware::gnss::V2_0::IAGnss interface follow.
   * These declarations were generated from IAGnss.hal.
   */
  Return<void> setCallback(const sp<V2_0::IAGnssCallback>& callback) override;

  Return<bool> dataConnClosed() override;

  Return<bool> dataConnFailed() override;

  Return<bool> dataConnOpen(uint64_t networkHandle, const hidl_string& apn,
                            V2_0::IAGnss::ApnIpType apnIpType) override;

  Return<bool> setServer(V2_0::IAGnssCallback::AGnssType type,
                         const hidl_string& hostname, int32_t port) override;

  void statusCb(AGpsExtType type, LocAGpsStatusValue status);

  /* Data call setup callback passed down to GNSS HAL implementation */
  static void agnssStatusIpV4Cb(AGnssExtStatusIpV4 status);

 private:
  Gnss* mGnss = nullptr;
  sp<IAGnssCallback> mAGnssCbIface = nullptr;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V2_0_AGNSS_H
