/* Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#include "BatchingAdapter.h"
#include "location_interface.h"

static BatchingAdapter* gBatchingAdapter = NULL;

static void initialize();
static void deinitialize();

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks);
static void removeClient(LocationAPI* client,
                         removeClientCompleteCallback rmClientCb);
static void requestCapabilities(LocationAPI* client);

static uint32_t startBatching(LocationAPI* client, BatchingOptions&);
static void stopBatching(LocationAPI* client, uint32_t id);
static void updateBatchingOptions(LocationAPI* client, uint32_t id,
                                  BatchingOptions&);
static void getBatchedLocations(LocationAPI* client, uint32_t id, size_t count);

static const BatchingInterface gBatchingInterface = {sizeof(BatchingInterface),
                                                     initialize,
                                                     deinitialize,
                                                     addClient,
                                                     removeClient,
                                                     requestCapabilities,
                                                     startBatching,
                                                     stopBatching,
                                                     updateBatchingOptions,
                                                     getBatchedLocations};

#ifndef DEBUG_X86
extern "C" const BatchingInterface* getBatchingInterface()
#else
const BatchingInterface* getBatchingInterface()
#endif  // DEBUG_X86
{
  return &gBatchingInterface;
}

static void initialize() {
  if (NULL == gBatchingAdapter) {
    gBatchingAdapter = new BatchingAdapter();
  }
}

static void deinitialize() {
  if (NULL != gBatchingAdapter) {
    delete gBatchingAdapter;
    gBatchingAdapter = NULL;
  }
}

static void addClient(LocationAPI* client, const LocationCallbacks& callbacks) {
  if (NULL != gBatchingAdapter) {
    gBatchingAdapter->addClientCommand(client, callbacks);
  }
}

static void removeClient(LocationAPI* client,
                         removeClientCompleteCallback rmClientCb) {
  if (NULL != gBatchingAdapter) {
    gBatchingAdapter->removeClientCommand(client, rmClientCb);
  }
}

static void requestCapabilities(LocationAPI* client) {
  if (NULL != gBatchingAdapter) {
    gBatchingAdapter->requestCapabilitiesCommand(client);
  }
}

static uint32_t startBatching(LocationAPI* client,
                              BatchingOptions& batchOptions) {
  if (NULL != gBatchingAdapter) {
    return gBatchingAdapter->startBatchingCommand(client, batchOptions);
  } else {
    return 0;
  }
}

static void stopBatching(LocationAPI* client, uint32_t id) {
  if (NULL != gBatchingAdapter) {
    gBatchingAdapter->stopBatchingCommand(client, id);
  }
}

static void updateBatchingOptions(LocationAPI* client, uint32_t id,
                                  BatchingOptions& batchOptions) {
  if (NULL != gBatchingAdapter) {
    gBatchingAdapter->updateBatchingOptionsCommand(client, id, batchOptions);
  }
}

static void getBatchedLocations(LocationAPI* client, uint32_t id,
                                size_t count) {
  if (NULL != gBatchingAdapter) {
    gBatchingAdapter->getBatchedLocationsCommand(client, id, count);
  }
}
