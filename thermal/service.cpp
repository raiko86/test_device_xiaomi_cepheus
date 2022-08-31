/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include "Thermal.h"
constexpr std::string_view kThermalLogTag("pixel-thermal");
using ::android::OK;
using ::android::status_t;
// libhwbinder:
using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;
// Generated HIDL files:
using ::android::hardware::thermal::V2_0::IThermal;
using ::android::hardware::thermal::V2_0::implementation::Thermal;
static int shutdown() {
    LOG(ERROR) << "Xiaomi Thermal HAL Service is shutting down.";
    return 1;
}
int main(int /* argc */, char ** /* argv */) {
    android::base::SetDefaultTag(kThermalLogTag.data());
    status_t status;
    android::sp<IThermal> service = nullptr;
    LOG(INFO) << "Xiaomi Thermal HAL Service 2.0 starting...";
    service = new Thermal();
    if (service == nullptr) {
        LOG(ERROR) << "Error creating an instance of Thermal HAL. Exiting...";
        return shutdown();
    }
    configureRpcThreadpool(1, true /* callerWillJoin */);
    status = service->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for ThermalHAL (" << status << ")";
        return shutdown();
    }
    LOG(INFO) << "Xiaomi Thermal HAL Service 2.0 started successfully.";
    joinRpcThreadpool();
    // We should not get past the joinRpcThreadpool().
    return shutdown();
}
