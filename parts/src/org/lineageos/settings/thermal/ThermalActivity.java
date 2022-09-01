/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.thermal;

import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.view.MenuItem;

public class ThermalActivity extends PreferenceActivity {
    private static final String TAG_THERMAL = "thermal";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getActionBar().setDisplayHomeAsUpEnabled(true);

        getFragmentManager()
                .beginTransaction()
                .replace(android.R.id.content, new ThermalSettingsFragment(), TAG_THERMAL)
                .commit();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
