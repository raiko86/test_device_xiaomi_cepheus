/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <PowerStatsAidl.h>
#include <utils/Looper.h>
#include <utils/Thread.h>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <thread>
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
          class DisplayStateResidencyDataProvider : public PowerStats::IStateResidencyDataProvider
          {
          public:
            // name = powerEntityName to be associated with this data provider
            // path = path to the display state file descriptor
            // state = list of states to be tracked
            DisplayStateResidencyDataProvider(std::string name, std::string path,
                                              std::vector<std::string> states);
            ~DisplayStateResidencyDataProvider();
            // Methods from PowerStats::IStateResidencyDataProvider
            bool getStateResidencies(
                std::unordered_map<std::string, std::vector<StateResidency>> *residencies) override;
            std::unordered_map<std::string, std::vector<State>> getInfo() override;
          private:
            // Poll for display state changes
            void pollLoop();
            // Main function to update the stats when display state change is detected
            void updateStats();
            // File descriptor of display state
            int mFd;
            // Path to display state file descriptor
            const std::string mPath;
            // Power Entity name associated with this data provider
            const std::string mName;
            // List of states to track indexed by mCurState
            std::vector<std::string> mStates;
            // Lock to protect concurrent read/write to mResidencies and mCurState
            std::mutex mLock;
            // Accumulated display state stats indexed by mCurState
            std::vector<StateResidency> mResidencies;
            // Index of current state
            int mCurState;
            // Looper to facilitate polling of display state file desciptor
            ::android::sp<::android::Looper> mLooper;
            std::thread mThread;
          };
        } // namespace stats
      }   // namespace power
    }     // namespace hardware
  }       // namespace android
} // namespace aidl
