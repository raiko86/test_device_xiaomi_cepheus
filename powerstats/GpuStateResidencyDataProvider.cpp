/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#define LOG_TAG "libpixelpowerstats"
#include "GpuStateResidencyDataProvider.h"
#include <android-base/logging.h>
#include <fstream>
#include <sstream>
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
                    GpuStateResidencyDataProvider::GpuStateResidencyDataProvider(uint32_t id)
                        : mPowerEntityId(id), mActiveId(0) /* (TODO (b/117228832): enable this) , mSuspendId(1) */ {}
                    bool GpuStateResidencyDataProvider::getTotalTime(const std::string &path, uint64_t &totalTimeMs)
                    {
                        std::ifstream inFile(path, std::ifstream::in);
                        if (!inFile.is_open())
                        {
                            PLOG(ERROR) << __func__ << ":Failed to open file " << path;
                            return false;
                        }
                        std::string line;
                        std::getline(inFile, line);
                        std::istringstream lineStream(line, std::istringstream::in);
                        totalTimeMs = 0;
                        uint64_t curTimeMs = 0;
                        while (lineStream >> curTimeMs)
                        {
                            totalTimeMs += curTimeMs;
                        }
                        return true;
                    }
                    bool GpuStateResidencyDataProvider::getResults(
                        std::unordered_map<uint32_t, PowerEntityStateResidencyResult> &results)
                    {
                        uint64_t totalActiveTimeUs = 0;
                        if (!getTotalTime("/sys/class/kgsl/kgsl-3d0/gpu_clock_stats", totalActiveTimeUs))
                        {
                            LOG(ERROR) << __func__ << "Failed to get results for GPU:Active";
                            return false;
                        }
                        /* (TODO (b/117228832): enable this)
                        uint64_t totalSuspendTimeMs = 0;
                        if (!getTotalTime("/sys/class/kgsl/kgsl-3d0/devfreq/suspend_time", totalSuspendTimeMs)) {
                            LOG(ERROR) << __func__ << "Failed to get results for GPU:Suspend";
                            return false;
                        }
                        */
                        PowerEntityStateResidencyResult result = {
                            .powerEntityId = mPowerEntityId,
                            .stateResidencyData = {
                                {.powerEntityStateId = mActiveId, .totalTimeInStateMs = totalActiveTimeUs / 1000},
                                /* (TODO (b/117228832): enable this)
                                {.powerEntityStateId = mSuspendId, .totalTimeInStateMs = totalSuspendTimeMs},
                                */
                            }};
                        results.emplace(std::make_pair(mPowerEntityId, result));
                        return true;
                    }
                    std::vector<PowerEntityStateSpace> GpuStateResidencyDataProvider::getStateSpaces()
                    {
                        return {{.powerEntityId = mPowerEntityId,
                                 .states = {
                                     {.powerEntityStateId = mActiveId, .powerEntityStateName = "Active"},
                                     /* (TODO (b/117228832): enable this)
                                     {.powerEntityStateId = mSuspendId, .powerEntityStateName = "Suspend"}
                                     */
                                 }}};
                    }
                } // namespace powerstats
            }     // namespace pixel
        }         // namespace google
    }             // namespace hardware
} // namespace android