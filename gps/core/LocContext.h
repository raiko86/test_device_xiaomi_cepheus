/* Copyright (c) 2011-2014, 2017-2019 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#ifndef __LOC_CONTEXT__
#define __LOC_CONTEXT__

#include <ContextBase.h>
#include <ctype.h>
#include <dlfcn.h>
#include <stdbool.h>

namespace loc_core {

class LocContext : public ContextBase {
  static const MsgTask* mMsgTask;
  static ContextBase* mContext;
  static const MsgTask* getMsgTask(LocThread::tCreate tCreator,
                                   const char* name, bool joinable = true);
  static const MsgTask* getMsgTask(const char* name, bool joinable = true);
  static pthread_mutex_t mGetLocContextMutex;

 protected:
  LocContext(const MsgTask* msgTask);
  inline virtual ~LocContext() {}

 public:
  static const char* mLBSLibName;
  static const char* mLocationHalName;

  static ContextBase* getLocContext(LocThread::tCreate tCreator,
                                    LocMsg* firstMsg, const char* name,
                                    bool joinable = true);
  inline static ContextBase* getLocContext(const char* name,
                                           bool joinable = true) {
    return getLocContext(NULL, NULL, name, joinable);
  }

  static void injectFeatureConfig(ContextBase* context);
};

}  // namespace loc_core

#endif  //__LOC_CONTEXT__
