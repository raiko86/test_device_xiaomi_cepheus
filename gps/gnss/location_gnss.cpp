/* Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#include "GnssAdapter.h"
#include "location_interface.h"

static GnssAdapter* gGnssAdapter = NULL;

static void initialize();
static void deinitialize();

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks);
static void removeClient(LocationAPI* client,
                         removeClientCompleteCallback rmClientCb);
static void requestCapabilities(LocationAPI* client);

static uint32_t startTracking(LocationAPI* client, TrackingOptions&);
static void updateTrackingOptions(LocationAPI* client, uint32_t id,
                                  TrackingOptions&);
static void stopTracking(LocationAPI* client, uint32_t id);

static void gnssNiResponse(LocationAPI* client, uint32_t id,
                           GnssNiResponse response);
static uint32_t gnssDeleteAidingData(GnssAidingData& data);
static void gnssUpdateXtraThrottle(const bool enabled);

static void setControlCallbacks(LocationControlCallbacks& controlCallbacks);
static uint32_t enable(LocationTechnologyType techType);
static void disable(uint32_t id);
static uint32_t* gnssUpdateConfig(const GnssConfig& config);
static uint32_t* gnssGetConfig(GnssConfigFlagsMask mask);

static void gnssUpdateSvTypeConfig(GnssSvTypeConfig& config);
static void gnssGetSvTypeConfig(GnssSvTypeConfigCallback& callback);
static void gnssResetSvTypeConfig();

static void injectLocation(double latitude, double longitude, float accuracy);
static void injectLocationExt(const GnssLocationInfoNotification& locationInfo);
static void injectTime(int64_t time, int64_t timeReference,
                       int32_t uncertainty);

static void agpsInit(const AgpsCbInfo& cbInfo);
static void agpsDataConnOpen(AGpsExtType agpsType, const char* apnName,
                             int apnLen, int ipType);
static void agpsDataConnClosed(AGpsExtType agpsType);
static void agpsDataConnFailed(AGpsExtType agpsType);
static void getDebugReport(GnssDebugReport& report);
static void updateConnectionStatus(
    bool connected, int8_t type, bool roaming = false,
    NetworkHandle networkHandle = NETWORK_HANDLE_UNKNOWN);
static void getGnssEnergyConsumed(GnssEnergyConsumedCallback energyConsumedCb);
static void enableNfwLocationAccess(bool enable);
static void nfwInit(const NfwCbInfo& cbInfo);
static void getPowerStateChanges(std::function<void(bool)> powerStateCb);

static void odcpiInit(const OdcpiRequestCallback& callback,
                      OdcpiPrioritytype priority);
static void odcpiInject(const Location& location);

static void blockCPI(double latitude, double longitude, float accuracy,
                     int blockDurationMsec, double latLonDiffThreshold);
static void updateBatteryStatus(bool charging);
static void updateSystemPowerState(PowerStateType systemPowerState);
static uint32_t setConstrainedTunc(bool enable, float tuncConstraint,
                                   uint32_t energyBudget);
static uint32_t setPositionAssistedClockEstimator(bool enable);
static uint32_t gnssUpdateSvConfig(
    const GnssSvTypeConfig& constellationEnablementConfig,
    const GnssSvIdConfig& blacklistSvConfig);
static uint32_t gnssResetSvConfig();
static uint32_t configLeverArm(const LeverArmConfigInfo& configInfo);
static uint32_t configRobustLocation(bool enable, bool enableForE911);
static uint32_t configMinGpsWeek(uint16_t minGpsWeek);
static uint32_t configDeadReckoningEngineParams(
    const DeadReckoningEngineConfig& dreConfig);
static uint32_t gnssUpdateSecondaryBandConfig(
    const GnssSvTypeConfig& secondaryBandConfig);
static uint32_t gnssGetSecondaryBandConfig();

static void updateNTRIPGGAConsent(bool consentAccepted);
static void enablePPENtripStream(const GnssNtripConnectionParams& params,
                                 bool enableRTKEngine);
static void disablePPENtripStream();

static bool measCorrInit(const measCorrSetCapabilitiesCb setCapabilitiesCb);
static bool measCorrSetCorrections(
    const GnssMeasurementCorrections gnssMeasCorr);
static void measCorrClose();
static uint32_t antennaInfoInit(const antennaInfoCb antennaInfoCallback);
static void antennaInfoClose();

static const GnssInterface gGnssInterface = {
    sizeof(GnssInterface),
    initialize,
    deinitialize,
    addClient,
    removeClient,
    requestCapabilities,
    startTracking,
    updateTrackingOptions,
    stopTracking,
    gnssNiResponse,
    setControlCallbacks,
    enable,
    disable,
    gnssUpdateConfig,
    gnssGetConfig,
    gnssUpdateSvTypeConfig,
    gnssGetSvTypeConfig,
    gnssResetSvTypeConfig,
    gnssDeleteAidingData,
    gnssUpdateXtraThrottle,
    injectLocation,
    injectTime,
    agpsInit,
    agpsDataConnOpen,
    agpsDataConnClosed,
    agpsDataConnFailed,
    getDebugReport,
    updateConnectionStatus,
    odcpiInit,
    odcpiInject,
    blockCPI,
    getGnssEnergyConsumed,
    enableNfwLocationAccess,
    nfwInit,
    getPowerStateChanges,
    injectLocationExt,
    updateBatteryStatus,
    updateSystemPowerState,
    setConstrainedTunc,
    setPositionAssistedClockEstimator,
    gnssUpdateSvConfig,
    configLeverArm,
    measCorrInit,
    measCorrSetCorrections,
    measCorrClose,
    antennaInfoInit,
    antennaInfoClose,
    configRobustLocation,
    configMinGpsWeek,
    configDeadReckoningEngineParams,
    updateNTRIPGGAConsent,
    enablePPENtripStream,
    disablePPENtripStream,
    gnssUpdateSecondaryBandConfig,
    gnssGetSecondaryBandConfig,
};

#ifndef DEBUG_X86
extern "C" const GnssInterface* getGnssInterface()
#else
const GnssInterface* getGnssInterface()
#endif  // DEBUG_X86
{
  gGnssInterface.initialize();
  return &gGnssInterface;
}

static void initialize() {
  if (NULL == gGnssAdapter) {
    gGnssAdapter = new GnssAdapter();
  }
}

static void deinitialize() {
  if (NULL != gGnssAdapter) {
    delete gGnssAdapter;
    gGnssAdapter = NULL;
  }
}

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->addClientCommand(client, callbacks);
  }
}

static void removeClient(LocationAPI* client,
                         removeClientCompleteCallback rmClientCb) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->removeClientCommand(client, rmClientCb);
  }
}

static void requestCapabilities(LocationAPI* client) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->requestCapabilitiesCommand(client);
  }
}

static uint32_t startTracking(LocationAPI* client,
                              TrackingOptions& trackingOptions) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->startTrackingCommand(client, trackingOptions);
  } else {
    return 0;
  }
}

static void updateTrackingOptions(LocationAPI* client, uint32_t id,
                                  TrackingOptions& trackingOptions) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->updateTrackingOptionsCommand(client, id, trackingOptions);
  }
}

static void stopTracking(LocationAPI* client, uint32_t id) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->stopTrackingCommand(client, id);
  }
}

static void gnssNiResponse(LocationAPI* client, uint32_t id,
                           GnssNiResponse response) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->gnssNiResponseCommand(client, id, response);
  }
}

static void setControlCallbacks(LocationControlCallbacks& controlCallbacks) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->setControlCallbacksCommand(controlCallbacks);
  }
}

static uint32_t enable(LocationTechnologyType techType) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->enableCommand(techType);
  } else {
    return 0;
  }
}

static void disable(uint32_t id) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->disableCommand(id);
  }
}

static uint32_t* gnssUpdateConfig(const GnssConfig& config) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->gnssUpdateConfigCommand(config);
  } else {
    return NULL;
  }
}

static uint32_t* gnssGetConfig(GnssConfigFlagsMask mask) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->gnssGetConfigCommand(mask);
  } else {
    return NULL;
  }
}

static void gnssUpdateSvTypeConfig(GnssSvTypeConfig& config) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->gnssUpdateSvTypeConfigCommand(config);
  }
}

static void gnssGetSvTypeConfig(GnssSvTypeConfigCallback& callback) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->gnssGetSvTypeConfigCommand(callback);
  }
}

static void gnssResetSvTypeConfig() {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->gnssResetSvTypeConfigCommand();
  }
}

static uint32_t gnssDeleteAidingData(GnssAidingData& data) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->gnssDeleteAidingDataCommand(data);
  } else {
    return 0;
  }
}

static void gnssUpdateXtraThrottle(const bool enabled) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->gnssUpdateXtraThrottleCommand(enabled);
  }
}

static void injectLocation(double latitude, double longitude, float accuracy) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->injectLocationCommand(latitude, longitude, accuracy);
  }
}

static void injectTime(int64_t time, int64_t timeReference,
                       int32_t uncertainty) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->injectTimeCommand(time, timeReference, uncertainty);
  }
}

static void agpsInit(const AgpsCbInfo& cbInfo) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->initAgpsCommand(cbInfo);
  }
}
static void agpsDataConnOpen(AGpsExtType agpsType, const char* apnName,
                             int apnLen, int ipType) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->dataConnOpenCommand(agpsType, apnName, apnLen,
                                      (AGpsBearerType)ipType);
  }
}
static void agpsDataConnClosed(AGpsExtType agpsType) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->dataConnClosedCommand(agpsType);
  }
}
static void agpsDataConnFailed(AGpsExtType agpsType) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->dataConnFailedCommand(agpsType);
  }
}

static void getDebugReport(GnssDebugReport& report) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->getDebugReport(report);
  }
}

static void updateConnectionStatus(bool connected, int8_t type, bool roaming,
                                   NetworkHandle networkHandle) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->getSystemStatus()->eventConnectionStatus(
        connected, type, roaming, networkHandle);
  }
}

static void odcpiInit(const OdcpiRequestCallback& callback,
                      OdcpiPrioritytype priority) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->initOdcpiCommand(callback, priority);
  }
}

static void odcpiInject(const Location& location) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->injectOdcpiCommand(location);
  }
}

static void blockCPI(double latitude, double longitude, float accuracy,
                     int blockDurationMsec, double latLonDiffThreshold) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->blockCPICommand(latitude, longitude, accuracy,
                                  blockDurationMsec, latLonDiffThreshold);
  }
}

static void getGnssEnergyConsumed(GnssEnergyConsumedCallback energyConsumedCb) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->getGnssEnergyConsumedCommand(energyConsumedCb);
  }
}

static void enableNfwLocationAccess(bool enable) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->nfwControlCommand(enable);
  }
}

static void nfwInit(const NfwCbInfo& cbInfo) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->initNfwCommand(cbInfo);
  }
}

static void getPowerStateChanges(std::function<void(bool)> powerStateCb) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->getPowerStateChangesCommand(powerStateCb);
  }
}

static void injectLocationExt(
    const GnssLocationInfoNotification& locationInfo) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->injectLocationExtCommand(locationInfo);
  }
}

static void updateBatteryStatus(bool charging) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->getSystemStatus()->updatePowerConnectState(charging);
  }
}

static void updateSystemPowerState(PowerStateType systemPowerState) {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->updateSystemPowerStateCommand(systemPowerState);
  }
}

static uint32_t setConstrainedTunc(bool enable, float tuncConstraint,
                                   uint32_t energyBudget) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->setConstrainedTuncCommand(enable, tuncConstraint,
                                                   energyBudget);
  } else {
    return 0;
  }
}

static uint32_t setPositionAssistedClockEstimator(bool enable) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->setPositionAssistedClockEstimatorCommand(enable);
  } else {
    return 0;
  }
}

static uint32_t gnssUpdateSvConfig(
    const GnssSvTypeConfig& constellationEnablementConfig,
    const GnssSvIdConfig& blacklistSvConfig) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->gnssUpdateSvConfigCommand(
        constellationEnablementConfig, blacklistSvConfig);
  } else {
    return 0;
  }
}

static uint32_t configLeverArm(const LeverArmConfigInfo& configInfo) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->configLeverArmCommand(configInfo);
  } else {
    return 0;
  }
}

static bool measCorrInit(const measCorrSetCapabilitiesCb setCapabilitiesCb) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->openMeasCorrCommand(setCapabilitiesCb);
  } else {
    return false;
  }
}

static bool measCorrSetCorrections(
    const GnssMeasurementCorrections gnssMeasCorr) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->measCorrSetCorrectionsCommand(gnssMeasCorr);
  } else {
    return false;
  }
}

static void measCorrClose() {
  if (NULL != gGnssAdapter) {
    gGnssAdapter->closeMeasCorrCommand();
  }
}

static uint32_t antennaInfoInit(const antennaInfoCb antennaInfoCallback) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->antennaInfoInitCommand(antennaInfoCallback);
  } else {
    return ANTENNA_INFO_ERROR_GENERIC;
  }
}

static void antennaInfoClose() {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->antennaInfoCloseCommand();
  }
}

static uint32_t configRobustLocation(bool enable, bool enableForE911) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->configRobustLocationCommand(enable, enableForE911);
  } else {
    return 0;
  }
}

static uint32_t configMinGpsWeek(uint16_t minGpsWeek) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->configMinGpsWeekCommand(minGpsWeek);
  } else {
    return 0;
  }
}

static uint32_t configDeadReckoningEngineParams(
    const DeadReckoningEngineConfig& dreConfig) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->configDeadReckoningEngineParamsCommand(dreConfig);
  } else {
    return 0;
  }
}

static uint32_t gnssUpdateSecondaryBandConfig(
    const GnssSvTypeConfig& secondaryBandConfig) {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->gnssUpdateSecondaryBandConfigCommand(
        secondaryBandConfig);
  } else {
    return 0;
  }
}

static uint32_t gnssGetSecondaryBandConfig() {
  if (NULL != gGnssAdapter) {
    return gGnssAdapter->gnssGetSecondaryBandConfigCommand();
  } else {
    return 0;
  }
}

static void updateNTRIPGGAConsent(bool consentAccepted) {
  if (NULL != gGnssAdapter) {
    // Call will be enabled once GnssAdapter impl. is ready.
    gGnssAdapter->updateNTRIPGGAConsentCommand(consentAccepted);
  }
}

static void enablePPENtripStream(const GnssNtripConnectionParams& params,
                                 bool enableRTKEngine) {
  if (NULL != gGnssAdapter) {
    // Call will be enabled once GnssAdapter impl. is ready.
    gGnssAdapter->enablePPENtripStreamCommand(params, enableRTKEngine);
  }
}

static void disablePPENtripStream() {
  if (NULL != gGnssAdapter) {
    // Call will be enabled once GnssAdapter impl. is ready.
    gGnssAdapter->disablePPENtripStreamCommand();
  }
}
