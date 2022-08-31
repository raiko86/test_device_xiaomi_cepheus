/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
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
          /**
           * This class provides the necessary functionality for selection of energy meter
           * based on file configurations.
           */
          class IioEnergyMeterDataSelector
          {
          public:
            IioEnergyMeterDataSelector(const std::unordered_map<std::string, std::string> &devicePaths);
          private:
            void parseConfigData(const std::string data,
                                 std::unordered_map<std::string, std::list<std::string>> *deviceConfigs);
            void applyConfigToDevices(
                const std::unordered_map<std::string, std::list<std::string>> &deviceConfigs);
            void applyConfigsByAscendingPriority();
            void sendConfigurationComplete();
            const std::unordered_map<std::string, std::string> kDevicePaths;
            const std::string kSelectionNode = "/enabled_rails";
            const std::string kSelectionComplete = "CONFIG_COMPLETE";
            /* Order matters (ascending priority), see applyConfigsByAscendingPriority() */
            const std::vector<const std::string> kConfigPaths = {
                "/data/vendor/powerstats/odpm_config",
            };
          };
        } // namespace stats
      }   // namespace power
    }     // namespace hardware
  }       // namespace android
} // namespace aidl
