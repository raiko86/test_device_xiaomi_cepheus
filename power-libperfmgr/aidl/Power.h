/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <atomic>
#include <memory>
#include <thread>
#include <aidl/android/hardware/power/BnPower.h>
#include <perfmgr/HintManager.h>
#include "disp-power/DisplayLowPower.h"
#include "disp-power/InteractionHandler.h"
namespace aidl
{
  namespace google
  {
    namespace hardware
    {
      namespace power
      {
        namespace impl
        {
          namespace pixel
          {
            using ::aidl::android::hardware::power::Boost;
            using ::aidl::android::hardware::power::IPowerHintSession;
            using ::aidl::android::hardware::power::Mode;
            using ::android::perfmgr::HintManager;
            class Power : public ::aidl::android::hardware::power::BnPower
            {
            public:
              Power(std::shared_ptr<HintManager> hm, std::shared_ptr<DisplayLowPower> dlpw);
              ndk::ScopedAStatus setMode(Mode type, bool enabled) override;
              ndk::ScopedAStatus isModeSupported(Mode type, bool *_aidl_return) override;
              ndk::ScopedAStatus setBoost(Boost type, int32_t durationMs) override;
              ndk::ScopedAStatus isBoostSupported(Boost type, bool *_aidl_return) override;
              ndk::ScopedAStatus createHintSession(int32_t tgid, int32_t uid,
                                                   const std::vector<int32_t> &threadIds,
                                                   int64_t durationNanos,
                                                   std::shared_ptr<IPowerHintSession> *_aidl_return) override;
              ndk::ScopedAStatus getHintSessionPreferredRate(int64_t *outNanoseconds) override;
            private:
              std::shared_ptr<HintManager> mHintManager;
              std::shared_ptr<DisplayLowPower> mDisplayLowPower;
              std::unique_ptr<InteractionHandler> mInteractionHandler;
              std::atomic<bool> mSustainedPerfModeOn;
              const int64_t mAdpfRateNs;
            };
          } // namespace pixel
        }   // namespace impl
      }     // namespace power
    }       // namespace hardware
  }         // namespace google
} // namespace aidl
