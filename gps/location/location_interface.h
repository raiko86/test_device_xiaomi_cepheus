/* Copyright (c) 2017-2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */

#ifndef LOCATION_INTERFACE_H
#define LOCATION_INTERFACE_H

#include <LocationAPI.h>
#include <gps_extended_c.h>

#include <functional>

/* Used for callback to deliver GNSS energy consumed */
/** @fn
    @brief Used by query API that retrieves energy consumed by
           modem GNSS engine.

    @param gnssEnergyConsumedFromFirstBoot:
            Energy consumed by the GNSS engine since the first bootup
            in units of 0.1 milli watt seconds.
            A value of 0xffffffffffffffff indicates an invalid reading.
*/
typedef std::function<void(uint64_t gnssEnergyConsumedFromFirstBoot)>
    GnssEnergyConsumedCallback;

typedef void (*removeClientCompleteCallback)(LocationAPI* client);

struct GnssInterface {
  size_t size;
  void (*initialize)(void);
  void (*deinitialize)(void);
  void (*addClient)(LocationAPI* client, const LocationCallbacks& callbacks);
  void (*removeClient)(LocationAPI* client,
                       removeClientCompleteCallback rmClientCb);
  void (*requestCapabilities)(LocationAPI* client);
  uint32_t (*startTracking)(LocationAPI* client, TrackingOptions&);
  void (*updateTrackingOptions)(LocationAPI* client, uint32_t id,
                                TrackingOptions&);
  void (*stopTracking)(LocationAPI* client, uint32_t id);
  void (*gnssNiResponse)(LocationAPI* client, uint32_t id,
                         GnssNiResponse response);
  void (*setControlCallbacks)(LocationControlCallbacks& controlCallbacks);
  uint32_t (*enable)(LocationTechnologyType techType);
  void (*disable)(uint32_t id);
  uint32_t* (*gnssUpdateConfig)(const GnssConfig& config);
  uint32_t* (*gnssGetConfig)(GnssConfigFlagsMask config);
  void (*gnssUpdateSvTypeConfig)(GnssSvTypeConfig& config);
  void (*gnssGetSvTypeConfig)(GnssSvTypeConfigCallback& callback);
  void (*gnssResetSvTypeConfig)();
  uint32_t (*gnssDeleteAidingData)(GnssAidingData& data);
  void (*gnssUpdateXtraThrottle)(const bool enabled);
  void (*injectLocation)(double latitude, double longitude, float accuracy);
  void (*injectTime)(int64_t time, int64_t timeReference, int32_t uncertainty);
  void (*agpsInit)(const AgpsCbInfo& cbInfo);
  void (*agpsDataConnOpen)(AGpsExtType agpsType, const char* apnName,
                           int apnLen, int ipType);
  void (*agpsDataConnClosed)(AGpsExtType agpsType);
  void (*agpsDataConnFailed)(AGpsExtType agpsType);
  void (*getDebugReport)(GnssDebugReport& report);
  void (*updateConnectionStatus)(bool connected, int8_t type, bool roaming,
                                 NetworkHandle networkHandle);
  void (*odcpiInit)(const OdcpiRequestCallback& callback,
                    OdcpiPrioritytype priority);
  void (*odcpiInject)(const Location& location);
  void (*blockCPI)(double latitude, double longitude, float accuracy,
                   int blockDurationMsec, double latLonDiffThreshold);
  void (*getGnssEnergyConsumed)(GnssEnergyConsumedCallback energyConsumedCb);
  void (*enableNfwLocationAccess)(bool enable);
  void (*nfwInit)(const NfwCbInfo& cbInfo);
  void (*getPowerStateChanges)(std::function<void(bool)> powerStateCb);
  void (*injectLocationExt)(const GnssLocationInfoNotification& locationInfo);
  void (*updateBatteryStatus)(bool charging);
  void (*updateSystemPowerState)(PowerStateType systemPowerState);
  uint32_t (*setConstrainedTunc)(bool enable, float tuncConstraint,
                                 uint32_t energyBudget);
  uint32_t (*setPositionAssistedClockEstimator)(bool enable);
  uint32_t (*gnssUpdateSvConfig)(
      const GnssSvTypeConfig& constellationEnablementConfig,
      const GnssSvIdConfig& blacklistSvConfig);
  uint32_t (*configLeverArm)(const LeverArmConfigInfo& configInfo);
  bool (*measCorrInit)(const measCorrSetCapabilitiesCb setCapabilitiesCb);
  bool (*measCorrSetCorrections)(const GnssMeasurementCorrections gnssMeasCorr);
  void (*measCorrClose)();
  uint32_t (*antennaInfoInit)(const antennaInfoCb antennaInfoCallback);
  void (*antennaInfoClose)();
  uint32_t (*configRobustLocation)(bool enable, bool enableForE911);
  uint32_t (*configMinGpsWeek)(uint16_t minGpsWeek);
  uint32_t (*configDeadReckoningEngineParams)(
      const DeadReckoningEngineConfig& dreConfig);
  void (*updateNTRIPGGAConsent)(bool consentAccepted);
  void (*enablePPENtripStream)(const GnssNtripConnectionParams& params,
                               bool enableRTKEngine);
  void (*disablePPENtripStream)();
  uint32_t (*gnssUpdateSecondaryBandConfig)(
      const GnssSvTypeConfig& secondaryBandConfig);
  uint32_t (*gnssGetSecondaryBandConfig)();
};

struct BatchingInterface {
  size_t size;
  void (*initialize)(void);
  void (*deinitialize)(void);
  void (*addClient)(LocationAPI* client, const LocationCallbacks& callbacks);
  void (*removeClient)(LocationAPI* client,
                       removeClientCompleteCallback rmClientCb);
  void (*requestCapabilities)(LocationAPI* client);
  uint32_t (*startBatching)(LocationAPI* client, BatchingOptions&);
  void (*stopBatching)(LocationAPI* client, uint32_t id);
  void (*updateBatchingOptions)(LocationAPI* client, uint32_t id,
                                BatchingOptions&);
  void (*getBatchedLocations)(LocationAPI* client, uint32_t id, size_t count);
};

struct GeofenceInterface {
  size_t size;
  void (*initialize)(void);
  void (*deinitialize)(void);
  void (*addClient)(LocationAPI* client, const LocationCallbacks& callbacks);
  void (*removeClient)(LocationAPI* client,
                       removeClientCompleteCallback rmClientCb);
  void (*requestCapabilities)(LocationAPI* client);
  uint32_t* (*addGeofences)(LocationAPI* client, size_t count, GeofenceOption*,
                            GeofenceInfo*);
  void (*removeGeofences)(LocationAPI* client, size_t count, uint32_t* ids);
  void (*modifyGeofences)(LocationAPI* client, size_t count, uint32_t* ids,
                          GeofenceOption* options);
  void (*pauseGeofences)(LocationAPI* client, size_t count, uint32_t* ids);
  void (*resumeGeofences)(LocationAPI* client, size_t count, uint32_t* ids);
};

#endif /* LOCATION_INTERFACE_H */
