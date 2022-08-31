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
          class GenericStateResidencyDataProvider : public PowerStats::IStateResidencyDataProvider
          {
          public:
            class StateResidencyConfig
            {
            public:
              std::string name;
              std::string header;
              bool entryCountSupported;
              std::string entryCountPrefix;
              std::function<uint64_t(uint64_t)> entryCountTransform;
              bool totalTimeSupported;
              std::string totalTimePrefix;
              std::function<uint64_t(uint64_t)> totalTimeTransform;
              bool lastEntrySupported;
              std::string lastEntryPrefix;
              std::function<uint64_t(uint64_t)> lastEntryTransform;
            };
            class PowerEntityConfig
            {
            public:
              PowerEntityConfig(const std::vector<StateResidencyConfig> &stateResidencyConfigs,
                                const std::string &name, const std::string &header = "")
                  : mStateResidencyConfigs(std::move(stateResidencyConfigs)),
                    mName(std::move(name)),
                    mHeader(std::move(header)) {}
              const std::vector<StateResidencyConfig> mStateResidencyConfigs;
              const std::string mName;
              const std::string mHeader;
            };
            GenericStateResidencyDataProvider(const std::string &path,
                                              const std::vector<PowerEntityConfig> &configs)
                : mPath(std::move(path)), mPowerEntityConfigs(std::move(configs)) {}
            ~GenericStateResidencyDataProvider() = default;
            // Methods from PowerStats::IStateResidencyDataProvider
            bool getStateResidencies(
                std::unordered_map<std::string, std::vector<StateResidency>> *residencies) override;
            std::unordered_map<std::string, std::vector<State>> getInfo() override;
          private:
            const std::string mPath;
            const std::vector<PowerEntityConfig> mPowerEntityConfigs;
          };
          std::vector<GenericStateResidencyDataProvider::StateResidencyConfig>
          generateGenericStateResidencyConfigs(
              const GenericStateResidencyDataProvider::StateResidencyConfig &stateConfig,
              const std::vector<std::pair<std::string, std::string>> &stateHeaders);
        } // namespace stats
      }   // namespace power
    }     // namespace hardware
  }       // namespace android
} // namespace aidl
