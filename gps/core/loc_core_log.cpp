/* Copyright (c) 2011-2015, 2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#define LOG_NDEBUG 0
#define LOG_TAG "LocSvc_core_log"

#include <loc_core_log.h>
#include <loc_log.h>
#include <loc_pla.h>
#include <log_util.h>

void LocPosMode::logv() const {
  LOC_LOGV(
      "Position mode: %s\n  Position recurrence: %s\n  "
      "min interval: %d\n  preferred accuracy: %d\n  "
      "preferred time: %d\n  credentials: %s  provider: %s \n  "
      "power mode: %d\n  tbm %d",
      loc_get_position_mode_name(mode),
      loc_get_position_recurrence_name(recurrence), min_interval,
      preferred_accuracy, preferred_time, credentials, provider, powerMode,
      timeBetweenMeasurements);
}

/* GPS status names */
DECLARE_TBL(gps_status_name) = {
    NAME_VAL(LOC_GPS_STATUS_NONE),
    NAME_VAL(LOC_GPS_STATUS_SESSION_BEGIN),
    NAME_VAL(LOC_GPS_STATUS_SESSION_END),
    NAME_VAL(LOC_GPS_STATUS_ENGINE_ON),
    NAME_VAL(LOC_GPS_STATUS_ENGINE_OFF),
};

/* Find Android GPS status name */
const char* loc_get_gps_status_name(LocGpsStatusValue gps_status) {
  return loc_get_name_from_val(gps_status_name_tbl, (int64_t)gps_status);
}

DECLARE_TBL(loc_eng_position_modes) = {NAME_VAL(LOC_POSITION_MODE_STANDALONE),
                                       NAME_VAL(LOC_POSITION_MODE_MS_BASED),
                                       NAME_VAL(LOC_POSITION_MODE_MS_ASSISTED),
                                       NAME_VAL(LOC_POSITION_MODE_RESERVED_1),
                                       NAME_VAL(LOC_POSITION_MODE_RESERVED_2),
                                       NAME_VAL(LOC_POSITION_MODE_RESERVED_3),
                                       NAME_VAL(LOC_POSITION_MODE_RESERVED_4),
                                       NAME_VAL(LOC_POSITION_MODE_RESERVED_5)};

const char* loc_get_position_mode_name(LocGpsPositionMode mode) {
  return loc_get_name_from_val(loc_eng_position_modes_tbl, (int64_t)mode);
}

DECLARE_TBL(loc_eng_position_recurrences) = {
    NAME_VAL(LOC_GPS_POSITION_RECURRENCE_PERIODIC),
    NAME_VAL(LOC_GPS_POSITION_RECURRENCE_SINGLE)};

const char* loc_get_position_recurrence_name(LocGpsPositionRecurrence recur) {
  return loc_get_name_from_val(loc_eng_position_recurrences_tbl,
                               (int64_t)recur);
}

const char* loc_get_aiding_data_mask_names(LocGpsAidingData /*data*/) {
  return NULL;
}

DECLARE_TBL(loc_eng_agps_types) = {
    NAME_VAL(LOC_AGPS_TYPE_INVALID), NAME_VAL(LOC_AGPS_TYPE_ANY),
    NAME_VAL(LOC_AGPS_TYPE_SUPL), NAME_VAL(LOC_AGPS_TYPE_C2K),
    NAME_VAL(LOC_AGPS_TYPE_WWAN_ANY)};

const char* loc_get_agps_type_name(LocAGpsType type) {
  return loc_get_name_from_val(loc_eng_agps_types_tbl, (int64_t)type);
}

DECLARE_TBL(loc_eng_ni_types) = {NAME_VAL(LOC_GPS_NI_TYPE_VOICE),
                                 NAME_VAL(LOC_GPS_NI_TYPE_UMTS_SUPL),
                                 NAME_VAL(LOC_GPS_NI_TYPE_UMTS_CTRL_PLANE),
                                 NAME_VAL(LOC_GPS_NI_TYPE_EMERGENCY_SUPL)};

const char* loc_get_ni_type_name(LocGpsNiType type) {
  return loc_get_name_from_val(loc_eng_ni_types_tbl, (int64_t)type);
}

DECLARE_TBL(loc_eng_ni_responses) = {NAME_VAL(LOC_GPS_NI_RESPONSE_ACCEPT),
                                     NAME_VAL(LOC_GPS_NI_RESPONSE_DENY),
                                     NAME_VAL(LOC_GPS_NI_RESPONSE_DENY)};

const char* loc_get_ni_response_name(LocGpsUserResponseType response) {
  return loc_get_name_from_val(loc_eng_ni_responses_tbl, (int64_t)response);
}

DECLARE_TBL(loc_eng_ni_encodings) = {
    NAME_VAL(LOC_GPS_ENC_NONE), NAME_VAL(LOC_GPS_ENC_SUPL_GSM_DEFAULT),
    NAME_VAL(LOC_GPS_ENC_SUPL_UTF8), NAME_VAL(LOC_GPS_ENC_SUPL_UCS2),
    NAME_VAL(LOC_GPS_ENC_UNKNOWN)};

const char* loc_get_ni_encoding_name(LocGpsNiEncodingType encoding) {
  return loc_get_name_from_val(loc_eng_ni_encodings_tbl, (int64_t)encoding);
}

DECLARE_TBL(loc_eng_agps_bears) = {
    NAME_VAL(AGPS_APN_BEARER_INVALID), NAME_VAL(AGPS_APN_BEARER_IPV4),
    NAME_VAL(AGPS_APN_BEARER_IPV6), NAME_VAL(AGPS_APN_BEARER_IPV4V6)};

const char* loc_get_agps_bear_name(AGpsBearerType bearer) {
  return loc_get_name_from_val(loc_eng_agps_bears_tbl, (int64_t)bearer);
}

DECLARE_TBL(loc_eng_server_types) = {
    NAME_VAL(LOC_AGPS_CDMA_PDE_SERVER), NAME_VAL(LOC_AGPS_CUSTOM_PDE_SERVER),
    NAME_VAL(LOC_AGPS_MPC_SERVER), NAME_VAL(LOC_AGPS_SUPL_SERVER)};

const char* loc_get_server_type_name(LocServerType type) {
  return loc_get_name_from_val(loc_eng_server_types_tbl, (int64_t)type);
}

DECLARE_TBL(loc_eng_position_sess_status_types) = {
    NAME_VAL(LOC_SESS_SUCCESS), NAME_VAL(LOC_SESS_INTERMEDIATE),
    NAME_VAL(LOC_SESS_FAILURE)};

const char* loc_get_position_sess_status_name(enum loc_sess_status status) {
  return loc_get_name_from_val(loc_eng_position_sess_status_types_tbl,
                               (int64_t)status);
}

DECLARE_TBL(loc_eng_agps_status_names) = {
    NAME_VAL(LOC_GPS_REQUEST_AGPS_DATA_CONN),
    NAME_VAL(LOC_GPS_RELEASE_AGPS_DATA_CONN),
    NAME_VAL(LOC_GPS_AGPS_DATA_CONNECTED),
    NAME_VAL(LOC_GPS_AGPS_DATA_CONN_DONE),
    NAME_VAL(LOC_GPS_AGPS_DATA_CONN_FAILED)};

const char* loc_get_agps_status_name(LocAGpsStatusValue status) {
  return loc_get_name_from_val(loc_eng_agps_status_names_tbl, (int64_t)status);
}
