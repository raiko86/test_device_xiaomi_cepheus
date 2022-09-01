/* Copyright (c) 2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#ifndef LOC_LOGGER_BASE_H
#define LOC_LOGGER_BASE_H

namespace loc_util {
class LocLoggerBase {
 public:
  virtual void log() {}
};
}  // namespace loc_util

#endif
