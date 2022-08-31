/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#define ATRACE_TAG (ATRACE_TAG_POWER | ATRACE_TAG_HAL)
#define LOG_TAG "android.hardware.power-service.pixel.ext-libperfmgr"
#include "PowerExt.h"
#include "PowerSessionManager.h"
#include <mutex>
#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <utils/Log.h>
#include <utils/Trace.h>
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
                        ndk::ScopedAStatus PowerExt::setMode(const std::string &mode, bool enabled)
                        {
                            LOG(DEBUG) << "PowerExt setMode: " << mode << " to: " << enabled;
                            ATRACE_INT(mode.c_str(), enabled);
                            if (enabled)
                            {
                                mHintManager->DoHint(mode);
                            }
                            else
                            {
                                mHintManager->EndHint(mode);
                            }
                            PowerSessionManager::getInstance()->updateHintMode(mode, enabled);
                            return ndk::ScopedAStatus::ok();
                        }
                        ndk::ScopedAStatus PowerExt::isModeSupported(const std::string &mode, bool *_aidl_return)
                        {
                            bool supported = mHintManager->IsHintSupported(mode);
                            LOG(INFO) << "PowerExt mode " << mode << " isModeSupported: " << supported;
                            *_aidl_return = supported;
                            return ndk::ScopedAStatus::ok();
                        }
                        ndk::ScopedAStatus PowerExt::setBoost(const std::string &boost, int32_t durationMs)
                        {
                            LOG(DEBUG) << "PowerExt setBoost: " << boost << " duration: " << durationMs;
                            ATRACE_INT(boost.c_str(), durationMs);
                            if (durationMs > 0)
                            {
                                mHintManager->DoHint(boost, std::chrono::milliseconds(durationMs));
                            }
                            else if (durationMs == 0)
                            {
                                mHintManager->DoHint(boost);
                            }
                            else
                            {
                                mHintManager->EndHint(boost);
                            }
                            return ndk::ScopedAStatus::ok();
                        }
                        ndk::ScopedAStatus PowerExt::isBoostSupported(const std::string &boost, bool *_aidl_return)
                        {
                            bool supported = mHintManager->IsHintSupported(boost);
                            LOG(INFO) << "PowerExt boost " << boost << " isBoostSupported: " << supported;
                            *_aidl_return = supported;
                            return ndk::ScopedAStatus::ok();
                        }
                    } // namespace pixel
                }     // namespace impl
            }         // namespace power
        }             // namespace hardware
    }                 // namespace google
} // namespace aidl
