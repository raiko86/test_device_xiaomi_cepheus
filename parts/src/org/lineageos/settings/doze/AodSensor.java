/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.doze;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class AodSensor implements SensorEventListener {
    private static final boolean DEBUG = false;
    private static final String TAG = "AodSensor";

    private SensorManager mSensorManager;
    private Sensor mSensor;
    private Context mContext;
    private ExecutorService mExecutorService;

    public AodSensor(Context context) {
        mContext = context;
        mSensorManager = mContext.getSystemService(SensorManager.class);
        mSensor = DozeUtils.getSensor(mSensorManager, "xiaomi.sensor.aod");
        mExecutorService = Executors.newSingleThreadExecutor();
    }

    private Future<?> submit(Runnable runnable) {
        return mExecutorService.submit(runnable);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (DEBUG) {
            Log.d(TAG, "Got sensor event: " + event.values[0]);
        }

        if (event.values[0] == 3 || event.values[0] == 5) {
            DozeUtils.setDozeMode(DozeUtils.DOZE_MODE_LBM);
        } else if (event.values[0] == 4) {
            DozeUtils.setDozeMode(DozeUtils.DOZE_MODE_HBM);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        /* Empty */
    }

    protected void enable() {
        if (DEBUG) {
            Log.d(TAG, "Enabling");
        }
        submit(() -> {
            mSensorManager.registerListener(this, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
        });
    }

    protected void disable() {
        if (DEBUG) {
            Log.d(TAG, "Disabling");
        }
        submit(() -> {
            mSensorManager.unregisterListener(this, mSensor);
        });
    }
}
