/* Copyright (c) 2012-2013, 2015-2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef LOC_ENG_NMEA_H
#define LOC_ENG_NMEA_H

#include <gps_extended.h>

#include <string>
#include <vector>
#define NMEA_SENTENCE_MAX_LENGTH 200

/** gnss datum type */
#define LOC_GNSS_DATUM_WGS84 0
#define LOC_GNSS_DATUM_PZ90 1

/* len of semi major axis of ref ellips*/
#define MAJA (6378137.0)
/* flattening coef of ref ellipsoid*/
#define FLAT (1.0 / 298.2572235630)
/* 1st eccentricity squared*/
#define ESQR (FLAT * (2.0 - FLAT))
/*1 minus eccentricity squared*/
#define OMES (1.0 - ESQR)
#define MILARCSEC2RAD (4.848136811095361e-09)
/*semi major axis */
#define C_PZ90A (6378136.0)
/*semi minor axis */
#define C_PZ90B (6356751.3618)
/* Transformation from WGS84 to PZ90
 * Cx,Cy,Cz,Rs,Rx,Ry,Rz,C_SYS_A,C_SYS_B*/
const double DatumConstFromWGS84[9] = {+0.003,
                                       +0.001,
                                       0.000,
                                       (1.0 + (0.000 * 1E-6)),
                                       (-0.019 * MILARCSEC2RAD),
                                       (+0.042 * MILARCSEC2RAD),
                                       (-0.002 * MILARCSEC2RAD),
                                       C_PZ90A,
                                       C_PZ90B};

/** Represents a LTP*/
typedef struct {
  double lat;
  double lon;
  double alt;
} LocLla;

/** Represents a ECEF*/
typedef struct {
  double X;
  double Y;
  double Z;
} LocEcef;

void loc_nmea_generate_sv(const GnssSvNotification &svNotify,
                          std::vector<std::string> &nmeaArraystr);

void loc_nmea_generate_pos(const UlpLocation &location,
                           const GpsLocationExtended &locationExtended,
                           const LocationSystemInfo &systemInfo,
                           unsigned char generate_nmea,
                           bool custom_gga_fix_quality,
                           std::vector<std::string> &nmeaArraystr,
                           int &indexOfGGA);

#define DEBUG_NMEA_MINSIZE 6
#define DEBUG_NMEA_MAXSIZE 4096
inline bool loc_nmea_is_debug(const char *nmea, int length) {
  return ((nullptr != nmea) && (length >= DEBUG_NMEA_MINSIZE) &&
          (length <= DEBUG_NMEA_MAXSIZE) && (nmea[0] == '$') &&
          (nmea[1] == 'P') && (nmea[2] == 'Q') && (nmea[3] == 'W'));
}

#endif  // LOC_ENG_NMEA_H
