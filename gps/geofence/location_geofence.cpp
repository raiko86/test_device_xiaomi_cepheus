/* Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#include "GeofenceAdapter.h"
#include "location_interface.h"

static GeofenceAdapter* gGeofenceAdapter = NULL;

static void initialize();
static void deinitialize();

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks);
static void removeClient(LocationAPI* client,
                         removeClientCompleteCallback rmClientCb);
static void requestCapabilities(LocationAPI* client);

static uint32_t* addGeofences(LocationAPI* client, size_t count,
                              GeofenceOption*, GeofenceInfo*);
static void removeGeofences(LocationAPI* client, size_t count, uint32_t* ids);
static void modifyGeofences(LocationAPI* client, size_t count, uint32_t* ids,
                            GeofenceOption* options);
static void pauseGeofences(LocationAPI* client, size_t count, uint32_t* ids);
static void resumeGeofences(LocationAPI* client, size_t count, uint32_t* ids);

static const GeofenceInterface gGeofenceInterface = {sizeof(GeofenceInterface),
                                                     initialize,
                                                     deinitialize,
                                                     addClient,
                                                     removeClient,
                                                     requestCapabilities,
                                                     addGeofences,
                                                     removeGeofences,
                                                     modifyGeofences,
                                                     pauseGeofences,
                                                     resumeGeofences};

#ifndef DEBUG_X86
extern "C" const GeofenceInterface* getGeofenceInterface()
#else
const GeofenceInterface* getGeofenceInterface()
#endif  // DEBUG_X86
{
  return &gGeofenceInterface;
}

static void initialize() {
  if (NULL == gGeofenceAdapter) {
    gGeofenceAdapter = new GeofenceAdapter();
  }
}

static void deinitialize() {
  if (NULL != gGeofenceAdapter) {
    delete gGeofenceAdapter;
    gGeofenceAdapter = NULL;
  }
}

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks) {
  if (NULL != gGeofenceAdapter) {
    gGeofenceAdapter->addClientCommand(client, callbacks);
  }
}

static void removeClient(LocationAPI* client,
                         removeClientCompleteCallback rmClientCb) {
  if (NULL != gGeofenceAdapter) {
    gGeofenceAdapter->removeClientCommand(client, rmClientCb);
  }
}

static void requestCapabilities(LocationAPI* client) {
  if (NULL != gGeofenceAdapter) {
    gGeofenceAdapter->requestCapabilitiesCommand(client);
  }
}

static uint32_t* addGeofences(LocationAPI* client, size_t count,
                              GeofenceOption* options, GeofenceInfo* info) {
  if (NULL != gGeofenceAdapter) {
    return gGeofenceAdapter->addGeofencesCommand(client, count, options, info);
  } else {
    return NULL;
  }
}

static void removeGeofences(LocationAPI* client, size_t count, uint32_t* ids) {
  if (NULL != gGeofenceAdapter) {
    return gGeofenceAdapter->removeGeofencesCommand(client, count, ids);
  }
}

static void modifyGeofences(LocationAPI* client, size_t count, uint32_t* ids,
                            GeofenceOption* options) {
  if (NULL != gGeofenceAdapter) {
    return gGeofenceAdapter->modifyGeofencesCommand(client, count, ids,
                                                    options);
  }
}

static void pauseGeofences(LocationAPI* client, size_t count, uint32_t* ids) {
  if (NULL != gGeofenceAdapter) {
    return gGeofenceAdapter->pauseGeofencesCommand(client, count, ids);
  }
}

static void resumeGeofences(LocationAPI* client, size_t count, uint32_t* ids) {
  if (NULL != gGeofenceAdapter) {
    return gGeofenceAdapter->resumeGeofencesCommand(client, count, ids);
  }
}
