/*
 * Copyright (C) 2015 The CyanogenMod Project
 *               2017-2019 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.util.Log;

import org.lineageos.settings.dirac.DiracUtils;
import org.lineageos.settings.doze.DozeUtils;
import org.lineageos.settings.thermal.ThermalUtils;

public class BootCompletedReceiver extends BroadcastReceiver {
    private static final boolean DEBUG = false;
    private static final String TAG = "XiaomiParts";

    @Override
    public void onReceive(final Context context, Intent intent) {
        if (DEBUG)
            Log.d(TAG, "Received boot completed intent");

        PendingResult pendingResult = goAsync();
        DiracUtils.initialize(context);
        DozeUtils.onBootCompleted(context);
        ThermalUtils.startService(context);

	pendingResult.finish();
    }
}
