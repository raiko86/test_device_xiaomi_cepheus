/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package android.vendor.powerstats;
import android.hardware.power.stats.StateResidency;
interface IPixelStateResidencyCallback
{
     StateResidency[] getStateResidency();
}
