/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "LocSvc_GnssNiInterface"

#include "GnssNi.h"

#include <log_util.h>

#include "Gnss.h"

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

void GnssNi::GnssNiDeathRecipient::serviceDied(uint64_t cookie,
                                               const wp<IBase>& who) {
  LOC_LOGE("%s] service died. cookie: %llu, who: %p", __FUNCTION__,
           static_cast<unsigned long long>(cookie), &who);
  // we do nothing here
  // Gnss::GnssDeathRecipient will stop the session
}

GnssNi::GnssNi(Gnss* gnss) : mGnss(gnss) {
  mGnssNiDeathRecipient = new GnssNiDeathRecipient(this);
}

// Methods from ::android::hardware::gnss::V1_0::IGnssNi follow.
Return<void> GnssNi::setCallback(const sp<IGnssNiCallback>& callback) {
  if (mGnss == nullptr) {
    LOC_LOGE("%s]: mGnss is nullptr", __FUNCTION__);
    return Void();
  }

  mGnss->setGnssNiCb(callback);

  if (mGnssNiCbIface != nullptr) {
    mGnssNiCbIface->unlinkToDeath(mGnssNiDeathRecipient);
  }
  mGnssNiCbIface = callback;
  if (mGnssNiCbIface != nullptr) {
    mGnssNiCbIface->linkToDeath(mGnssNiDeathRecipient, 0 /*cookie*/);
  }

  return Void();
}

Return<void> GnssNi::respond(
    int32_t notifId, IGnssNiCallback::GnssUserResponseType userResponse) {
  if (mGnss == nullptr) {
    LOC_LOGE("%s]: mGnss is nullptr", __FUNCTION__);
    return Void();
  }

  GnssAPIClient* api = mGnss->getApi();
  if (api == nullptr) {
    LOC_LOGE("%s]: api is nullptr", __FUNCTION__);
    return Void();
  }

  api->gnssNiRespond(notifId, userResponse);

  return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
