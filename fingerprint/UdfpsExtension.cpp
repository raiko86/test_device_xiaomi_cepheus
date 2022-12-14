/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <compositionengine/UdfpsExtension.h>
#include <drm/sde_drm.h>
#include <stdint.h>

uint32_t getUdfpsZOrder(uint32_t z, bool touched) {
  if (touched) {
    z |= FOD_PRESSED_LAYER_ZORDER;
  }

  return z;
}

uint64_t getUdfpsUsageBits(uint64_t usageBits, bool) { return usageBits; }
