/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package android.vendor.powerstats;
parcelable StateResidencyData {
  @utf8InCpp String state;
  long totalTimeInStateMs;
  long totalStateEntryCount;
  long lastEntryTimestampMs;
}
