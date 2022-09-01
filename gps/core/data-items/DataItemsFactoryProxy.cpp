/* Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#define LOG_TAG "DataItemsFactoryProxy"

#include <DataItemId.h>
#include <DataItemsFactoryProxy.h>
#include <IDataItemCore.h>
#include <dlfcn.h>
#include <loc_pla.h>
#include <log_util.h>

#include "loc_misc_utils.h"

namespace loc_core {
void* DataItemsFactoryProxy::dataItemLibHandle = NULL;
get_concrete_data_item_fn* DataItemsFactoryProxy::getConcreteDIFunc = NULL;

IDataItemCore* DataItemsFactoryProxy::createNewDataItem(DataItemId id) {
  IDataItemCore* mydi = nullptr;

  if (NULL != getConcreteDIFunc) {
    mydi = (*getConcreteDIFunc)(id);
  } else {
    getConcreteDIFunc = (get_concrete_data_item_fn*)dlGetSymFromLib(
        dataItemLibHandle, DATA_ITEMS_LIB_NAME, DATA_ITEMS_GET_CONCRETE_DI);

    if (NULL != getConcreteDIFunc) {
      LOC_LOGd("Loaded function %s : %p", DATA_ITEMS_GET_CONCRETE_DI,
               getConcreteDIFunc);
      mydi = (*getConcreteDIFunc)(id);
    } else {
      // dlysm failed.
      const char* err = dlerror();
      if (NULL == err) {
        err = "Unknown";
      }
      LOC_LOGe("failed to find symbol %s; error=%s", DATA_ITEMS_GET_CONCRETE_DI,
               err);
    }
  }
  return mydi;
}

void DataItemsFactoryProxy::closeDataItemLibraryHandle() {
  if (NULL != dataItemLibHandle) {
    dlclose(dataItemLibHandle);
    dataItemLibHandle = NULL;
  }
}

}  // namespace loc_core
