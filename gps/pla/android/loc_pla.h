/* Copyright (c) 2014, 2020 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __LOC_PLA__
#define __LOC_PLA__

#ifdef __cplusplus
#include <utils/SystemClock.h>
#define uptimeMillis() android::uptimeMillis()
#define elapsedRealtime() android::elapsedRealtime()
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <cutils/android_filesystem_config.h>
#include <cutils/properties.h>
#include <cutils/sched_policy.h>
#include <cutils/threads.h>
#include <stdlib.h>
#include <string.h>

#define UID_GPS (AID_GPS)
#define GID_GPS (AID_GPS)
#define UID_LOCCLIENT (4021)
#define GID_LOCCLIENT (4021)

#define LOC_PATH_GPS_CONF_STR "/vendor/etc/gps.conf"
#define LOC_PATH_IZAT_CONF_STR "/vendor/etc/izat.conf"
#define LOC_PATH_FLP_CONF_STR "/vendor/etc/flp.conf"
#define LOC_PATH_LOWI_CONF_STR "/vendor/etc/lowi.conf"
#define LOC_PATH_SAP_CONF_STR "/vendor/etc/sap.conf"
#define LOC_PATH_APDR_CONF_STR "/vendor/etc/apdr.conf"
#define LOC_PATH_XTWIFI_CONF_STR "/vendor/etc/xtwifi.conf"
#define LOC_PATH_QUIPC_CONF_STR "/vendor/etc/quipc.conf"
#define LOC_PATH_ANT_CORR_STR "/vendor/etc/gnss_antenna_info.conf"
#define LOC_PATH_SLIM_CONF_STR "/vendor/etc/slim.conf"
#define LOC_PATH_VPE_CONF_STR "/vendor/etc/vpeglue.conf"

/*!
 * @brief Function for memory block copy
 *
 * @param[out] p_Dest     Destination buffer.
 * @param[in]  q_DestSize Destination buffer size.
 * @param[in]  p_Src      Source buffer.
 * @param[in]  q_SrcSize  Source buffer size.
 *
 * @return Number of bytes copied.
 */
static inline size_t memscpy(void *p_Dest, size_t q_DestSize, const void *p_Src,
                             size_t q_SrcSize) {
  size_t res = (q_DestSize < q_SrcSize) ? q_DestSize : q_SrcSize;
  if (p_Dest && p_Src && q_DestSize > 0 && q_SrcSize > 0) {
    memcpy(p_Dest, p_Src, res);
  } else {
    res = 0;
  }
  return res;
}

/*API for boot kpi marker prints  */
inline int loc_boot_kpi_marker(const char *pFmt, ...) { return -1; }

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* __LOC_PLA__ */
