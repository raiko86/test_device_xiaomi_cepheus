/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package android.vendor.powerstats;
import android.vendor.powerstats.IPixelPowerStatsCallback;
interface IPixelPowerStatsProvider
{
    void registerCallback(in @utf8InCpp String entityName, in IPixelPowerStatsCallback callback);
    void unregisterCallback(in IPixelPowerStatsCallback callback);
}
