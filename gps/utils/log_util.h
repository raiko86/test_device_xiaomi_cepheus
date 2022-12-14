/* Copyright (c) 2011-2014 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef __LOG_UTIL_H__
#define __LOG_UTIL_H__

#include <loc_pla.h>
#include <stdbool.h>

#if defined(USE_ANDROID_LOGGING) || defined(ANDROID)
// Android and LE targets with logcat support
#include <sys/syscall.h>
#include <unistd.h>
#include <utils/Log.h>

#elif defined(USE_GLIB)
// LE targets with no logcat support
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef LOG_TAG
#define LOG_TAG "GPS_UTILS"
#endif /* LOG_TAG */

// LE targets with no logcat support
#ifdef FEATURE_EXTERNAL_AP
#include <syslog.h>
#define ALOGE(...) syslog(LOG_ERR, "LOC_LOGE: " __VA_ARGS__);
#define ALOGW(...) syslog(LOG_WARNING, "LOC_LOGW: " __VA_ARGS__);
#define ALOGI(...) syslog(LOG_NOTICE, "LOC_LOGI: " __VA_ARGS__);
#define ALOGD(...) syslog(LOG_DEBUG, "LOC_LOGD: " __VA_ARGS__);
#define ALOGV(...) syslog(LOG_NOTICE, "LOC_LOGV: " __VA_ARGS__);
#else /* FEATURE_EXTERNAL_AP */
#define TS_PRINTF(format, x...)                                      \
  {                                                                  \
    struct timeval tv;                                               \
    struct timezone tz;                                              \
    int hh, mm, ss;                                                  \
    gettimeofday(&tv, &tz);                                          \
    hh = tv.tv_sec / 3600 % 24;                                      \
    mm = (tv.tv_sec % 3600) / 60;                                    \
    ss = tv.tv_sec % 60;                                             \
    fprintf(stdout, "%02d:%02d:%02d.%06ld]" format "\n", hh, mm, ss, \
            tv.tv_usec, ##x);                                        \
  }

#define ALOGE(format, x...) \
  TS_PRINTF("E/%s (%d): " format, LOG_TAG, getpid(), ##x)
#define ALOGW(format, x...) \
  TS_PRINTF("W/%s (%d): " format, LOG_TAG, getpid(), ##x)
#define ALOGI(format, x...) \
  TS_PRINTF("I/%s (%d): " format, LOG_TAG, getpid(), ##x)
#define ALOGD(format, x...) \
  TS_PRINTF("D/%s (%d): " format, LOG_TAG, getpid(), ##x)
#define ALOGV(format, x...) \
  TS_PRINTF("V/%s (%d): " format, LOG_TAG, getpid(), ##x)
#endif /* FEATURE_EXTERNAL_AP */

#endif /* #if defined (USE_ANDROID_LOGGING) || defined (ANDROID) */

#ifdef __cplusplus
extern "C" {
#endif
/*=============================================================================
 *
 *                         LOC LOGGER TYPE DECLARATION
 *
 *============================================================================*/
/* LOC LOGGER */
typedef struct loc_logger_s {
  unsigned long DEBUG_LEVEL;
  unsigned long TIMESTAMP;
  bool LOG_BUFFER_ENABLE;
} loc_logger_s_type;

/*=============================================================================
 *
 *                               EXTERNAL DATA
 *
 *============================================================================*/

// Logging Improvements
extern const char *loc_logger_boolStr[];

extern loc_logger_s_type loc_logger;
extern const char *boolStr[];
extern const char VOID_RET[];
extern const char FROM_AFW[];
extern const char TO_MODEM[];
extern const char FROM_MODEM[];
extern const char TO_AFW[];
extern const char EXIT_TAG[];
extern const char ENTRY_TAG[];
extern const char EXIT_ERROR_TAG[];

#define BUILD_TYPE_PROP_NA 0
#define BUILD_TYPE_PROP_USER 1
#define BUILD_TYPE_PROP_USERDEBUG 2
#define BUILD_TYPE_PROP_INVALID 3
extern int build_type_prop;

/*=============================================================================
 *
 *                        MODULE EXPORTED FUNCTIONS
 *
 *============================================================================*/
inline void loc_logger_init(unsigned long debug, unsigned long timestamp) {
  loc_logger.DEBUG_LEVEL = debug;

  if (BUILD_TYPE_PROP_NA == build_type_prop) {
    char value[PROPERTY_VALUE_MAX] = "NA";
    property_get("ro.build.type", value, "userdebug");
    if (0 == strcmp(value, "user")) {
      build_type_prop = BUILD_TYPE_PROP_USER;
    } else if (0 == strcmp(value, "userdebug")) {
      build_type_prop = BUILD_TYPE_PROP_USERDEBUG;
    } else {
      build_type_prop = BUILD_TYPE_PROP_INVALID;
    }
  }

  if (BUILD_TYPE_PROP_USER == build_type_prop) {
    // force user builds to 2 or less
    if (loc_logger.DEBUG_LEVEL > 2) {
      loc_logger.DEBUG_LEVEL = 2;
    }
  }

  loc_logger.TIMESTAMP = timestamp;
}

inline void log_buffer_init(bool enabled) {
  loc_logger.LOG_BUFFER_ENABLE = enabled;
}

extern char *get_timestamp(char *str, unsigned long buf_size);
extern void log_buffer_insert(char *str, unsigned long buf_size, int level);

/*=============================================================================
 *
 *                          LOGGING BUFFER MACROS
 *
 *============================================================================*/
#ifndef LOG_NDEBUG
#define LOG_NDEBUG 0
#endif
#define TOTAL_LOG_LEVELS 5
#define LOGGING_BUFFER_MAX_LEN 1024
#define IF_LOG_BUFFER_ENABLE if (loc_logger.LOG_BUFFER_ENABLE)
#define INSERT_BUFFER(flag, level, format, x...)                            \
  {                                                                         \
    IF_LOG_BUFFER_ENABLE {                                                  \
      if (flag == 0) {                                                      \
        char timestr[32];                                                   \
        get_timestamp(timestr, sizeof(timestr));                            \
        char log_str[LOGGING_BUFFER_MAX_LEN];                               \
        snprintf(log_str, LOGGING_BUFFER_MAX_LEN,                           \
                 "%s %d %ld %s :" format "\n", timestr, getpid(),           \
                 syscall(SYS_gettid), LOG_TAG == NULL ? "" : LOG_TAG, ##x); \
        log_buffer_insert(log_str, sizeof(log_str), level);                 \
      }                                                                     \
    }                                                                       \
  }

#ifndef DEBUG_DMN_LOC_API

/* LOGGING MACROS */
/*loc_logger.DEBUG_LEVEL is initialized to 0xff in loc_cfg.cpp
  if that value remains unchanged, it means gps.conf did not
  provide a value and we default to the initial value to use
  Android's logging levels*/
#define IF_LOC_LOGE \
  if ((loc_logger.DEBUG_LEVEL >= 1) && (loc_logger.DEBUG_LEVEL <= 5))
#define IF_LOC_LOGW \
  if ((loc_logger.DEBUG_LEVEL >= 2) && (loc_logger.DEBUG_LEVEL <= 5))
#define IF_LOC_LOGI \
  if ((loc_logger.DEBUG_LEVEL >= 3) && (loc_logger.DEBUG_LEVEL <= 5))
#define IF_LOC_LOGD \
  if ((loc_logger.DEBUG_LEVEL >= 4) && (loc_logger.DEBUG_LEVEL <= 5))
#define IF_LOC_LOGV \
  if ((loc_logger.DEBUG_LEVEL >= 5) && (loc_logger.DEBUG_LEVEL <= 5))

#define LOC_LOGE(...)                          \
  IF_LOC_LOGE {                                \
    ALOGE(__VA_ARGS__);                        \
    INSERT_BUFFER(LOG_NDEBUG, 0, __VA_ARGS__); \
  }
#define LOC_LOGW(...)                          \
  IF_LOC_LOGW {                                \
    ALOGW(__VA_ARGS__);                        \
    INSERT_BUFFER(LOG_NDEBUG, 1, __VA_ARGS__); \
  }
#define LOC_LOGI(...)                          \
  IF_LOC_LOGI {                                \
    ALOGI(__VA_ARGS__);                        \
    INSERT_BUFFER(LOG_NDEBUG, 2, __VA_ARGS__); \
  }
#define LOC_LOGD(...)                          \
  IF_LOC_LOGD {                                \
    ALOGD(__VA_ARGS__);                        \
    INSERT_BUFFER(LOG_NDEBUG, 3, __VA_ARGS__); \
  }
#define LOC_LOGV(...)                          \
  IF_LOC_LOGV {                                \
    ALOGV(__VA_ARGS__);                        \
    INSERT_BUFFER(LOG_NDEBUG, 4, __VA_ARGS__); \
  }

#else /* DEBUG_DMN_LOC_API */

#define LOC_LOGE(...) ALOGE(__VA_ARGS__)
#define LOC_LOGW(...) ALOGW(__VA_ARGS__)
#define LOC_LOGI(...) ALOGI(__VA_ARGS__)
#define LOC_LOGD(...) ALOGD(__VA_ARGS__)
#define LOC_LOGV(...) ALOGV(__VA_ARGS__)

#endif /* DEBUG_DMN_LOC_API */

/*=============================================================================
 *
 *                          LOGGING IMPROVEMENT MACROS
 *
 *============================================================================*/
#define LOG_(LOC_LOG, ID, WHAT, SPEC, VAL)                                    \
  do {                                                                        \
    if (loc_logger.TIMESTAMP) {                                               \
      char ts[32];                                                            \
      LOC_LOG("[%s] %s %s line %d " #SPEC, get_timestamp(ts, sizeof(ts)), ID, \
              WHAT, __LINE__, VAL);                                           \
    } else {                                                                  \
      LOC_LOG("%s %s line %d " #SPEC, ID, WHAT, __LINE__, VAL);               \
    }                                                                         \
  } while (0)

#define LOC_LOG_HEAD(fmt) "%s:%d] " fmt
#define LOC_LOGv(fmt, ...) \
  LOC_LOGV(LOC_LOG_HEAD(fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOC_LOGw(fmt, ...) \
  LOC_LOGW(LOC_LOG_HEAD(fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOC_LOGi(fmt, ...) \
  LOC_LOGI(LOC_LOG_HEAD(fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOC_LOGd(fmt, ...) \
  LOC_LOGD(LOC_LOG_HEAD(fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOC_LOGe(fmt, ...) \
  LOC_LOGE(LOC_LOG_HEAD(fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_I(ID, WHAT, SPEC, VAL) LOG_(LOC_LOGI, ID, WHAT, SPEC, VAL)
#define LOG_V(ID, WHAT, SPEC, VAL) LOG_(LOC_LOGV, ID, WHAT, SPEC, VAL)
#define LOG_E(ID, WHAT, SPEC, VAL) LOG_(LOC_LOGE, ID, WHAT, SPEC, VAL)
#define LOG_D(ID, WHAT, SPEC, VAL) LOG_(LOC_LOGD, ID, WHAT, SPEC, VAL)

#define ENTRY_LOG() LOG_V(ENTRY_TAG, __FUNCTION__, % s, "")
#define EXIT_LOG(SPEC, VAL) LOG_V(EXIT_TAG, __FUNCTION__, SPEC, VAL)
#define EXIT_LOG_WITH_ERROR(SPEC, VAL)              \
  if (VAL != 0) {                                   \
    LOG_E(EXIT_ERROR_TAG, __FUNCTION__, SPEC, VAL); \
  } else {                                          \
    LOG_V(EXIT_TAG, __FUNCTION__, SPEC, VAL);       \
  }

// Used for logging callflow from Android Framework
#define ENTRY_LOG_CALLFLOW() LOG_I(FROM_AFW, __FUNCTION__, % s, "")
// Used for logging callflow to Modem
#define EXIT_LOG_CALLFLOW(SPEC, VAL) LOG_I(TO_MODEM, __FUNCTION__, SPEC, VAL)
// Used for logging callflow from Modem(TO_MODEM, __FUNCTION__, %s, "")
#define MODEM_LOG_CALLFLOW(SPEC, VAL) LOG_I(FROM_MODEM, __FUNCTION__, SPEC, VAL)
// Used for logging high frequency callflow from Modem(TO_MODEM, __FUNCTION__,
// %s, "")
#define MODEM_LOG_CALLFLOW_DEBUG(SPEC, VAL) \
  LOG_D(FROM_MODEM, __FUNCTION__, SPEC, VAL)
// Used for logging callflow to Android Framework
#define CALLBACK_LOG_CALLFLOW(CB, SPEC, VAL) LOG_I(TO_AFW, CB, SPEC, VAL)

#ifdef __cplusplus
}
#endif

#endif  // __LOG_UTIL_H__
