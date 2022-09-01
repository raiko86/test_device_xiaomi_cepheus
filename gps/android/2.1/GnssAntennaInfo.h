/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */

#ifndef ANDROID_HARDWARE_GNSS_V2_1_GNSSANTENNAINFO_H
#define ANDROID_HARDWARE_GNSS_V2_1_GNSSANTENNAINFO_H

#include <android/hardware/gnss/2.1/IGnssAntennaInfo.h>
#include <hidl/Status.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V2_1 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::gnss::V2_1::IGnssAntennaInfo;
using ::android::hardware::gnss::V2_1::IGnssAntennaInfoCallback;

struct Gnss;
struct GnssAntennaInfo : public IGnssAntennaInfo {
  GnssAntennaInfo(Gnss* gnss);
  ~GnssAntennaInfo();

  /*
   * Methods from ::android::hardware::gnss::V1_1::IGnssAntennaInfo follow.
   * These declarations were generated from IGnssAntennaInfo.hal.
   */
  Return<GnssAntennaInfoStatus> setCallback(
      const sp<IGnssAntennaInfoCallback>& callback) override;
  Return<void> close(void) override;

  void gnssAntennaInfoCb(
      std::vector<GnssAntennaInformation> gnssAntennaInformations);

  static void aiGnssAntennaInfoCb(
      std::vector<GnssAntennaInformation> gnssAntennaInformations);

 private:
  struct GnssAntennaInfoDeathRecipient : hidl_death_recipient {
    GnssAntennaInfoDeathRecipient(sp<GnssAntennaInfo> gnssAntennaInfo)
        : mGnssAntennaInfo(gnssAntennaInfo) {}
    ~GnssAntennaInfoDeathRecipient() = default;
    virtual void serviceDied(uint64_t cookie, const wp<IBase>& who) override;
    sp<GnssAntennaInfo> mGnssAntennaInfo;
  };

 private:
  sp<GnssAntennaInfoDeathRecipient> mGnssAntennaInfoDeathRecipient = nullptr;
  sp<IGnssAntennaInfoCallback> mGnssAntennaInfoCbIface = nullptr;
  Gnss* mGnss = nullptr;
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V2_1_GNSSANTENNAINFO_H
