/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef HARDWARE_GOOGLE_PIXEL_POWERSTATS_GPUSTATERESIDENCYDATAPROVIDER_H
#define HARDWARE_GOOGLE_PIXEL_POWERSTATS_GPUSTATERESIDENCYDATAPROVIDER_H
#include <pixelpowerstats/PowerStats.h>
using android::hardware::power::stats::V1_0::PowerEntityStateResidencyResult;
using android::hardware::power::stats::V1_0::PowerEntityStateSpace;
namespace android
{
  namespace hardware
  {
    namespace google
    {
      namespace pixel
      {
        namespace powerstats
        {
          class GpuStateResidencyDataProvider : public IStateResidencyDataProvider
          {
          public:
            GpuStateResidencyDataProvider(uint32_t id);
            ~GpuStateResidencyDataProvider() = default;
            bool getResults(
                std::unordered_map<uint32_t, PowerEntityStateResidencyResult> &results) override;
            std::vector<PowerEntityStateSpace> getStateSpaces() override;
          private:
            bool getTotalTime(const std::string &path, uint64_t &totalTimeMs);
            const uint32_t mPowerEntityId;
            const uint32_t mActiveId;
            /* (TODO (b/117228832): enable this) const uint32_t mSuspendId; */
          };
        } // namespace powerstats
      }   // namespace pixel
    }     // namespace google
  }       // namespace hardware
} // namespace android
#endif // HARDWARE_GOOGLE_PIXEL_POWERSTATS_GPUSTATERESIDENCYDATAPROVIDER_H
