/*
 * Copyright (C) 2018,2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.dirac;

import android.app.ActionBar;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import androidx.preference.ListPreference;
import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;
import androidx.preference.PreferenceFragment;
import androidx.preference.SwitchPreference;

import org.lineageos.settings.R;

public class DiracSettingsFragment extends PreferenceFragment implements
        OnPreferenceChangeListener, CompoundButton.OnCheckedChangeListener {

    private static final String PREF_HEADSET = "dirac_headset_pref";
    private static final String PREF_HIFI = "dirac_hifi_pref";
    private static final String PREF_PRESET = "dirac_preset_pref";

    private TextView mTextView;
    private View mSwitchBar;

    private ListPreference mHeadsetType;
    private ListPreference mPreset;
    private SwitchPreference mHifi;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.dirac_settings);
        final ActionBar actionBar = getActivity().getActionBar();
        actionBar.setDisplayHomeAsUpEnabled(true);

        DiracUtils.initialize(getActivity());
        boolean enhancerEnabled = DiracUtils.isDiracEnabled();

        mHeadsetType = (ListPreference) findPreference(PREF_HEADSET);
        mHeadsetType.setOnPreferenceChangeListener(this);

        mPreset = (ListPreference) findPreference(PREF_PRESET);
        mPreset.setOnPreferenceChangeListener(this);

        mHifi = (SwitchPreference) findPreference(PREF_HIFI);
        mHifi.setOnPreferenceChangeListener(this);

        boolean hifiEnable = DiracUtils.getHifiMode();
        mHeadsetType.setEnabled(!hifiEnable && enhancerEnabled);
        mPreset.setEnabled(!hifiEnable && enhancerEnabled);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        final View view = LayoutInflater.from(getContext()).inflate(R.layout.dirac,
                container, false);
        ((ViewGroup) view).addView(super.onCreateView(inflater, container, savedInstanceState));
        return view;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        boolean enhancerEnabled = DiracUtils.isDiracEnabled();

        mTextView = view.findViewById(R.id.switch_text);
        mTextView.setText(getString(enhancerEnabled ?
                R.string.switch_bar_on : R.string.switch_bar_off));

        mSwitchBar = view.findViewById(R.id.switch_bar);
        Switch switchWidget = mSwitchBar.findViewById(android.R.id.switch_widget);
        switchWidget.setChecked(enhancerEnabled);
        switchWidget.setOnCheckedChangeListener(this);
        mSwitchBar.setActivated(enhancerEnabled);
        mSwitchBar.setOnClickListener(v -> {
            switchWidget.setChecked(!switchWidget.isChecked());
            mSwitchBar.setActivated(switchWidget.isChecked());
        });
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        switch (preference.getKey()) {
            case PREF_HEADSET:
                DiracUtils.setHeadsetType(Integer.parseInt(newValue.toString()));
                return true;
            case PREF_HIFI:
                DiracUtils.setHifiMode((Boolean) newValue ? 1 : 0);
                if (DiracUtils.isDiracEnabled()) {
                    mHeadsetType.setEnabled(!(Boolean) newValue);
                    mPreset.setEnabled(!(Boolean) newValue);
                }
                return true;
            case PREF_PRESET:
                DiracUtils.setLevel((String) newValue);
                return true;
            default:
                return false;
        }
    }

    @Override
    public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
        mTextView.setText(getString(isChecked ? R.string.switch_bar_on : R.string.switch_bar_off));
        mSwitchBar.setActivated(isChecked);

        DiracUtils.setMusic(isChecked);

        if (!DiracUtils.getHifiMode()) {
            mHeadsetType.setEnabled(isChecked);
            mPreset.setEnabled(isChecked);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            getActivity().onBackPressed();
            return true;
        }
        return false;
    }
}
