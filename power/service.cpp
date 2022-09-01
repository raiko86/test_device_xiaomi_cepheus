/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.power@1.3-service.xiaomi_msmnile"

#include <hidl/HidlTransportSupport.h>

#include "Power.h"

using android::sp;
using android::status_t;
using android::OK;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using ::android::hardware::power::V1_3::IPower;
using ::android::hardware::power::V1_3::implementation::Power;

int main() {
    ALOGI("Power HAL 1.3 service is starting");

    sp<IPower> service = new Power();
    if (service == nullptr) {
        ALOGE("Failed to create an instance of Power HAL, exiting");
        return 1;
    }
    configureRpcThreadpool(1, true /* callerWillJoin */);

    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Failed to register service for Power HAL, exiting");
        return 1;
    }

    ALOGI("Power HAL service is ready");
    joinRpcThreadpool();

    // In normal operation, we don't expect the thread pool to exit
    ALOGE("Power HAL service is shutting down");
    return 1;
}
