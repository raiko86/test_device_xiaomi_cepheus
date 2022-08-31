/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <PowerStatsAidl.h>
#include <unordered_map>
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
          class IioEnergyMeterDataProvider : public PowerStats::IEnergyMeterDataProvider
          {
          public:
            IioEnergyMeterDataProvider(const std::vector<const std::string> &deviceNames,
                                       const bool useSelector = false);
            // Methods from PowerStats::IRailEnergyDataProvider
            ndk::ScopedAStatus readEnergyMeter(const std::vector<int32_t> &in_channelIds,
                                               std::vector<EnergyMeasurement> *_aidl_return) override;
            ndk::ScopedAStatus getEnergyMeterInfo(std::vector<Channel> *_aidl_return) override;
          private:
            void findIioEnergyMeterNodes();
            void parseEnabledRails();
            int parseEnergyValue(std::string path);
            int parseEnergyContents(const std::string &contents);
            std::mutex mLock;
            std::unordered_map<std::string, std::string> mDevicePaths; // key: path, value: device name
            std::unordered_map<std::string, int32_t> mChannelIds;      // key: name, value: id
            std::vector<Channel> mChannelInfos;
            std::vector<EnergyMeasurement> mReading;
            const std::vector<const std::string> kDeviceNames;
            const std::string kDeviceType = "iio:device";
            const std::string kIioRootDir = "/sys/bus/iio/devices/";
            const std::string kNameNode = "/name";
            const std::string kEnabledRailsNode = "/enabled_rails";
            const std::string kEnergyValueNode = "/energy_value";
          };
        } // namespace stats
      }   // namespace power
    }     // namespace hardware
  }       // namespace android
} // namespace aidl
