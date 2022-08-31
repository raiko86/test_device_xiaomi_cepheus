/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <PowerStatsAidl.h>
namespace aidl
{
  namespace android
  {
    namespace hardware
    {
      namespace power
      {
        namespace stats
        {
          class WlanStateResidencyDataProvider : public PowerStats::IStateResidencyDataProvider
          {
          public:
            WlanStateResidencyDataProvider(std::string name, std::string path)
                : mName(std::move(name)), mPath(std::move(path)) {}
            ~WlanStateResidencyDataProvider() = default;
            // Methods from PowerStats::IStateResidencyDataProvider
            bool getStateResidencies(
                std::unordered_map<std::string, std::vector<StateResidency>> *residencies) override;
            std::unordered_map<std::string, std::vector<State>> getInfo() override;
          private:
            const std::string mName;
            const std::string mPath;
          };
        } // namespace stats
      }   // namespace power
    }     // namespace hardware
  }       // namespace android
} // namespace aidl
