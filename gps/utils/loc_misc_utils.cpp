/* Copyright (c) 2014, 2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#define LOG_NDEBUG 0
#define LOG_TAG "LocSvc_misc_utils"
#include <ctype.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <inttypes.h>
#include <loc_misc_utils.h>
#include <log_util.h>
#include <stdio.h>
#include <string.h>

#ifndef MSEC_IN_ONE_SEC
#define MSEC_IN_ONE_SEC 1000ULL
#endif
#define GET_MSEC_FROM_TS(ts) \
  ((ts.tv_sec * MSEC_IN_ONE_SEC) + (ts.tv_nsec + 500000) / 1000000)

int loc_util_split_string(char* raw_string, char** split_strings_ptr,
                          int max_num_substrings, char delimiter) {
  int raw_string_index = 0;
  int num_split_strings = 0;
  unsigned char end_string = 0;
  int raw_string_length = 0;

  if (!raw_string || !split_strings_ptr) {
    LOC_LOGE("%s:%d]: NULL parameters", __func__, __LINE__);
    num_split_strings = -1;
    goto err;
  }
  LOC_LOGD("%s:%d]: raw string: %s\n", __func__, __LINE__, raw_string);
  raw_string_length = strlen(raw_string) + 1;
  split_strings_ptr[num_split_strings] = &raw_string[raw_string_index];
  for (raw_string_index = 0; raw_string_index < raw_string_length;
       raw_string_index++) {
    if (raw_string[raw_string_index] == '\0') end_string = 1;
    if ((raw_string[raw_string_index] == delimiter) || end_string) {
      raw_string[raw_string_index] = '\0';
      if (num_split_strings < max_num_substrings) {
        LOC_LOGD("%s:%d]: split string: %s\n", __func__, __LINE__,
                 split_strings_ptr[num_split_strings]);
      }
      num_split_strings++;
      if (((raw_string_index + 1) < raw_string_length) &&
          (num_split_strings < max_num_substrings)) {
        split_strings_ptr[num_split_strings] =
            &raw_string[raw_string_index + 1];
      } else {
        break;
      }
    }
    if (end_string) break;
  }
err:
  LOC_LOGD("%s:%d]: num_split_strings: %d\n", __func__, __LINE__,
           num_split_strings);
  return num_split_strings;
}

void loc_util_trim_space(char* org_string) {
  char *scan_ptr, *write_ptr;
  char *first_nonspace = NULL, *last_nonspace = NULL;

  if (org_string == NULL) {
    LOC_LOGE("%s:%d]: NULL parameter", __func__, __LINE__);
    goto err;
  }

  scan_ptr = write_ptr = org_string;

  while (*scan_ptr) {
    // Find the first non-space character
    if (!isspace(*scan_ptr) && first_nonspace == NULL) {
      first_nonspace = scan_ptr;
    }
    // Once the first non-space character is found in the
    // above check, keep shifting the characters to the left
    // to replace the spaces
    if (first_nonspace != NULL) {
      *(write_ptr++) = *scan_ptr;
      // Keep track of which was the last non-space character
      // encountered
      // last_nonspace will not be updated in the case where
      // the string ends with spaces
      if (!isspace(*scan_ptr)) {
        last_nonspace = write_ptr;
      }
    }
    scan_ptr++;
  }
  // Add NULL terminator after the last non-space character
  if (last_nonspace) {
    *last_nonspace = '\0';
  }
err:
  return;
}

inline void logDlError(const char* failedCall) {
  const char* err = dlerror();
  LOC_LOGe("%s error: %s", failedCall, (nullptr == err) ? "unknown" : err);
}

void* dlGetSymFromLib(void*& libHandle, const char* libName,
                      const char* symName) {
  void* sym = nullptr;
  if ((nullptr != libHandle || nullptr != libName) && nullptr != symName) {
    if (nullptr == libHandle) {
      libHandle = dlopen(libName, RTLD_NOW);
      if (nullptr == libHandle) {
        logDlError("dlopen");
      }
    }
    // NOT else, as libHandle gets assigned 5 line above
    if (nullptr != libHandle) {
      sym = dlsym(libHandle, symName);
      if (nullptr == sym) {
        logDlError("dlsym");
      }
    }
  } else {
    LOC_LOGe(
        "Either libHandle (%p) or libName (%p) must not be null; "
        "symName (%p) can not be null.",
        libHandle, libName, symName);
  }

  return sym;
}

uint64_t getQTimerTickCount() {
  uint64_t qTimerCount = 0;
#if __aarch64__
  asm volatile("mrs %0, cntvct_el0" : "=r"(qTimerCount));
#elif defined(__i386__) || defined(__x86_64__)
  /* Qtimer not supported in x86 architecture */
  qTimerCount = 0;
#else
  asm volatile("mrrc p15, 1, %Q0, %R0, c14" : "=r"(qTimerCount));
#endif

  return qTimerCount;
}

uint64_t getQTimerDeltaNanos() {
  char qtimer_val_string[100];
  char* temp;
  uint64_t local_qtimer = 0, remote_qtimer = 0;
  int mdm_fd = -1, wlan_fd = -1, ret = 0;
  uint64_t delta = 0;

  memset(qtimer_val_string, '\0', sizeof(qtimer_val_string));

  char devNode[] = "/sys/bus/mhi/devices/0306_00.01.00/time_us";
  for (; devNode[27] < 3 && mdm_fd < 0; devNode[27]++) {
    mdm_fd = ::open(devNode, O_RDONLY);
    if (mdm_fd < 0) {
      LOC_LOGe("MDM open file: %s error: %s", devNode, strerror(errno));
    }
  }
  if (mdm_fd > 0) {
    ret = read(mdm_fd, qtimer_val_string, sizeof(qtimer_val_string) - 1);
    ::close(mdm_fd);
    if (ret < 0) {
      LOC_LOGe("MDM read time_us file error: %s", strerror(errno));
    } else {
      temp = qtimer_val_string;
      temp = strchr(temp, ':');
      temp = temp + 2;
      local_qtimer = atoll(temp);

      temp = strchr(temp, ':');
      temp = temp + 2;
      remote_qtimer = atoll(temp);

      if (local_qtimer >= remote_qtimer) {
        delta = (local_qtimer - remote_qtimer) * 1000;
      }
      LOC_LOGv("qtimer values in microseconds: local:%" PRIi64
               " remote:%" PRIi64
               ""
               " delta in nanoseconds:%" PRIi64 "",
               local_qtimer, remote_qtimer, delta);
    }
  }
  return delta;
}

uint64_t getQTimerFreq() {
#if __aarch64__
  uint64_t val = 0;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(val));
#elif defined(__i386__) || defined(__x86_64__)
  /* Qtimer not supported in x86 architecture */
  uint64_t val = 0;
#else
  uint32_t val = 0;
  asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r"(val));
#endif
  return val;
}

uint64_t getBootTimeMilliSec() {
  struct timespec curTs;
  clock_gettime(CLOCK_BOOTTIME, &curTs);
  return (uint64_t)GET_MSEC_FROM_TS(curTs);
}
