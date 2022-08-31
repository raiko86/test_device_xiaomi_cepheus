/*
 * Copyright (C) 2020 The Android Open Source Project
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <aidl/android/hardware/light/BnLights.h>
#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <map>
#include <sstream>
namespace aidl
{
  namespace android
  {
    namespace hardware
    {
      namespace light
      {
        class Lights : public BnLights
        {
        public:
          Lights();
          ndk::ScopedAStatus setLightState(int id, const HwLightState &state) override;
          ndk::ScopedAStatus getLights(std::vector<HwLight> *types) override;
        private:
          void setLightNotification(int id, const HwLightState &state);
          void applyNotificationState(const HwLightState &state);
          uint32_t max_led_brightness_;
          std::map<int, std::function<void(int id, const HwLightState &)>> mLights;
          std::vector<HwLight> mAvailableLights;
          // Keep sorted in the order of importance.
          std::array<std::pair<int, HwLightState>, 2> notif_states_ = {{
              {(int)LightType::NOTIFICATIONS, {}},
              {(int)LightType::BATTERY, {}},
          }};
        };
      } // namespace light
    }   // namespace hardware
  }     // namespace android
} // namespace aidl
