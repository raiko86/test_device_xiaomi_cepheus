/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.dirac;

import android.os.Bundle;
import android.preference.PreferenceActivity;

public class DiracActivity extends PreferenceActivity {

    private static final String TAG_DIRAC = "dirac";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getFragmentManager().beginTransaction().replace(android.R.id.content,
                new DiracSettingsFragment(), TAG_DIRAC).commit();
    }
}
