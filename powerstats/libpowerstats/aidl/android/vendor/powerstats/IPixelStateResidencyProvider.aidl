/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package android.vendor.powerstats;
import android.vendor.powerstats.IPixelStateResidencyCallback;
interface IPixelStateResidencyProvider
{
    void registerCallback(in @utf8InCpp String entityName, in IPixelStateResidencyCallback cb);
    void unregisterCallback(in IPixelStateResidencyCallback cb);
}
