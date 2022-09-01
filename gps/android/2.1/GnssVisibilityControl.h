/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */

#ifndef ANDROID_HARDWARE_GNSS_V1_0_GnssVisibilityControl_H
#define ANDROID_HARDWARE_GNSS_V1_0_GnssVisibilityControl_H

#include <android/hardware/gnss/visibility_control/1.0/IGnssVisibilityControl.h>
#include <gps_extended_c.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <location_interface.h>

#include "Gnss.h"

namespace android {
namespace hardware {
namespace gnss {
namespace visibility_control {
namespace V1_0 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::gnss::V2_1::implementation::Gnss;

struct GnssVisibilityControl : public IGnssVisibilityControl {
  GnssVisibilityControl(Gnss* gnss);
  ~GnssVisibilityControl();

  // Methods from
  // ::android::hardware::gnss::visibility_control::V1_0::IGnssVisibilityControl
  // follow.
  Return<bool> enableNfwLocationAccess(
      const hidl_vec<::android::hardware::hidl_string>& proxyApps) override;
  /**
   * Registers the callback for HAL implementation to use.
   *
   * @param callback Handle to IGnssVisibilityControlCallback interface.
   */
  Return<bool> setCallback(
      const ::android::sp<::android::hardware::gnss::visibility_control::V1_0::
                              IGnssVisibilityControlCallback>& callback)
      override;

  void statusCb(GnssNfwNotification notification);
  bool isE911Session();

  /* Data call setup callback passed down to GNSS HAL implementation */
  static void nfwStatusCb(GnssNfwNotification notification);
  static bool isInEmergencySession();

 private:
  Gnss* mGnss = nullptr;
  sp<IGnssVisibilityControlCallback> mGnssVisibilityControlCbIface = nullptr;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace visibility_control
}  // namespace gnss
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_GNSS_V1_0_GnssVisibilityControl_H
