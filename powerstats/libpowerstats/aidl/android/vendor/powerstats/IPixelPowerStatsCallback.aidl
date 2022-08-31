/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package android.vendor.powerstats;
import android.vendor.powerstats.StateResidencyData;
interface IPixelPowerStatsCallback
{
     void getStats(out StateResidencyData[] stats);
}
