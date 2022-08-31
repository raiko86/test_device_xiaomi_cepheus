/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <string_view>
#include <android-base/unique_fd.h>
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
            class DisplayLowPower
            {
            public:
              DisplayLowPower();
              ~DisplayLowPower() {}
              void Init();
              void SetDisplayLowPower(bool enable);
            private:
              void ConnectPpsDaemon();
              int SendPpsCommand(const std::string_view cmd);
              void SetFoss(bool enable);
              ::android::base::unique_fd mPpsSocket;
              bool mFossStatus;
            };
          } // namespace pixel
        }   // namespace impl
      }     // namespace power
    }       // namespace hardware
  }         // namespace google
} // namespace aidl
