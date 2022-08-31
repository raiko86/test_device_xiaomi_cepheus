/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#define LOG_TAG "powerhal-libperfmgr"
#include <errno.h>
#include <unistd.h>
#include <cutils/sockets.h>
#include <log/log.h>
#include "DisplayLowPower.h"
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
                        DisplayLowPower::DisplayLowPower() : mFossStatus(false) {}
                        void DisplayLowPower::Init()
                        {
                            ConnectPpsDaemon();
                        }
                        void DisplayLowPower::SetDisplayLowPower(bool enable)
                        {
                            SetFoss(enable);
                        }
                        void DisplayLowPower::ConnectPpsDaemon()
                        {
                            constexpr const char kPpsDaemon[] = "pps";
                            mPpsSocket.reset(
                                socket_local_client(kPpsDaemon, ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM));
                            if (mPpsSocket.get() < 0)
                            {
                                ALOGW("Connecting to PPS daemon failed (%s)", strerror(errno));
                            }
                        }
                        int DisplayLowPower::SendPpsCommand(const std::string_view cmd)
                        {
                            if (TEMP_FAILURE_RETRY(write(mPpsSocket.get(), cmd.data(), cmd.size())) < 0)
                            {
                                ALOGE("Failed to send pps command '%s' over socket (%s)", cmd.data(), strerror(errno));
                                return -1;
                            }
                            return 0;
                        }
                        void DisplayLowPower::SetFoss(bool enable)
                        {
                            if (mPpsSocket.get() < 0 || mFossStatus == enable)
                            {
                                return;
                            }
                            ALOGI("%s foss", (enable) ? "Enable" : "Disable");
                            std::string_view foss_cmd;
                            if (enable)
                            {
                                foss_cmd = "foss:on";
                            }
                            else
                            {
                                foss_cmd = "foss:off";
                            }
                            if (!SendPpsCommand(foss_cmd))
                            {
                                mFossStatus = enable;
                            }
                        }
                    } // namespace pixel
                }     // namespace impl
            }         // namespace power
        }             // namespace hardware
    }                 // namespace google
} // namespace aidl
