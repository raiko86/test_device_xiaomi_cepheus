/* Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef BATCHING_API_CLINET_H
#define BATCHING_API_CLINET_H

#include <LocationAPIClientBase.h>
#include <android/hardware/gnss/2.0/IGnssBatching.h>
#include <android/hardware/gnss/2.0/IGnssBatchingCallback.h>
#include <pthread.h>

#include <mutex>

namespace android {
namespace hardware {
namespace gnss {
namespace V2_0 {
namespace implementation {

class BatchingAPIClient : public LocationAPIClientBase {
 public:
  BatchingAPIClient(const sp<V1_0::IGnssBatchingCallback>& callback);
  BatchingAPIClient(const sp<V2_0::IGnssBatchingCallback>& callback);
  void gnssUpdateCallbacks(const sp<V1_0::IGnssBatchingCallback>& callback);
  void gnssUpdateCallbacks_2_0(const sp<V2_0::IGnssBatchingCallback>& callback);
  int getBatchSize();
  int startSession(const V1_0::IGnssBatching::Options& options);
  int updateSessionOptions(const V1_0::IGnssBatching::Options& options);
  int stopSession();
  void getBatchedLocation(int last_n_locations);
  void flushBatchedLocations();

  inline LocationCapabilitiesMask getCapabilities() {
    return mLocationCapabilitiesMask;
  }

  // callbacks
  void onCapabilitiesCb(LocationCapabilitiesMask capabilitiesMask) final;
  void onBatchingCb(size_t count, Location* location,
                    BatchingOptions batchOptions) final;

 private:
  void setCallbacks();
  ~BatchingAPIClient();

  std::mutex mMutex;
  sp<V1_0::IGnssBatchingCallback> mGnssBatchingCbIface;
  uint32_t mDefaultId;
  LocationCapabilitiesMask mLocationCapabilitiesMask;
  sp<V2_0::IGnssBatchingCallback> mGnssBatchingCbIface_2_0;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
#endif  // BATCHING_API_CLINET_H
