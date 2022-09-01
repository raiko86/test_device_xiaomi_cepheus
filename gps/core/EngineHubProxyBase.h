/* Copyright (c) 2018-2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#ifndef ENGINE_HUB_PROXY_BASE_H
#define ENGINE_HUB_PROXY_BASE_H

namespace loc_core {

class EngineHubProxyBase {
 public:
  inline EngineHubProxyBase() {}
  inline virtual ~EngineHubProxyBase() {}

  // gnss session related functions
  inline virtual bool gnssStartFix() { return false; }

  inline virtual bool gnssStopFix() { return false; }

  inline virtual bool gnssSetFixMode(const LocPosMode& params) {
    (void)params;
    return false;
  }

  inline virtual bool gnssDeleteAidingData(const GnssAidingData& aidingData) {
    (void)aidingData;
    return false;
  }

  // GNSS reports
  inline virtual bool gnssReportPosition(
      const UlpLocation& location, const GpsLocationExtended& locationExtended,
      enum loc_sess_status status) {
    (void)location;
    (void)locationExtended;
    (void)status;
    return false;
  }

  inline virtual bool gnssReportSv(const GnssSvNotification& svNotify) {
    (void)svNotify;
    return false;
  }

  inline virtual bool gnssReportSvMeasurement(
      const GnssSvMeasurementSet& svMeasurementSet) {
    (void)svMeasurementSet;
    return false;
  }

  inline virtual bool gnssReportSvPolynomial(
      const GnssSvPolynomial& svPolynomial) {
    (void)svPolynomial;
    return false;
  }

  inline virtual bool gnssReportSvEphemeris(
      const GnssSvEphemerisReport& svEphemeris) {
    (void)svEphemeris;
    return false;
  }

  inline virtual bool gnssReportSystemInfo(
      const LocationSystemInfo& systemInfo) {
    (void)systemInfo;
    return false;
  }

  inline virtual bool gnssReportKlobucharIonoModel(
      const GnssKlobucharIonoModel& ionoModel) {
    (void)ionoModel;
    return false;
  }

  inline virtual bool gnssReportAdditionalSystemInfo(
      const GnssAdditionalSystemInfo& additionalSystemInfo) {
    (void)additionalSystemInfo;
    return false;
  }

  inline virtual bool configLeverArm(const LeverArmConfigInfo& configInfo) {
    (void)configInfo;
    return false;
  }

  inline virtual bool configDeadReckoningEngineParams(
      const DeadReckoningEngineConfig& dreConfig) {
    (void)dreConfig;
    return false;
  }
};

typedef std::function<void(int count, EngineLocationInfo* locationArr)>
    GnssAdapterReportEnginePositionsEventCb;

typedef std::function<void(const GnssSvNotification& svNotify,
                           bool fromEngineHub)>
    GnssAdapterReportSvEventCb;

typedef std::function<void(const GnssAidingDataSvMask& svDataMask)>
    GnssAdapterReqAidingDataCb;

typedef std::function<void(bool nHzNeeded, bool nHzMeasNeeded)>
    GnssAdapterUpdateNHzRequirementCb;

// potential parameters: message queue: MsgTask * msgTask;
// callback function to report back dr and ppe position and sv report
typedef EngineHubProxyBase*(
    getEngHubProxyFn)(const MsgTask* msgTask, IOsObserver* osObserver,
                      GnssAdapterReportEnginePositionsEventCb positionEventCb,
                      GnssAdapterReportSvEventCb svEventCb,
                      GnssAdapterReqAidingDataCb reqAidingDataCb,
                      GnssAdapterUpdateNHzRequirementCb updateNHzRequirementCb);

}  // namespace loc_core

#endif  // ENGINE_HUB_PROXY_BASE_H
