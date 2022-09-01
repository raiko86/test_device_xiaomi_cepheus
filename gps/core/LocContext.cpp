/* Copyright (c) 2011-2014, 2016-2019 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#define LOG_NDEBUG 0
#define LOG_TAG "LocSvc_Ctx"

#include <LocContext.h>
#include <cutils/sched_policy.h>
#include <loc_log.h>
#include <log_util.h>
#include <msg_q.h>
#include <unistd.h>

namespace loc_core {

const MsgTask* LocContext::mMsgTask = NULL;
ContextBase* LocContext::mContext = NULL;
// the name must be shorter than 15 chars
const char* LocContext::mLocationHalName = "Loc_hal_worker";
#ifndef USE_GLIB
const char* LocContext::mLBSLibName = "liblbs_core.so";
#else
const char* LocContext::mLBSLibName = "liblbs_core.so.1";
#endif

pthread_mutex_t LocContext::mGetLocContextMutex = PTHREAD_MUTEX_INITIALIZER;

const MsgTask* LocContext::getMsgTask(LocThread::tCreate tCreator,
                                      const char* name, bool joinable) {
  if (NULL == mMsgTask) {
    mMsgTask = new MsgTask(tCreator, name, joinable);
  }
  return mMsgTask;
}

inline const MsgTask* LocContext::getMsgTask(const char* name, bool joinable) {
  return getMsgTask((LocThread::tCreate)NULL, name, joinable);
}

ContextBase* LocContext::getLocContext(LocThread::tCreate tCreator,
                                       LocMsg* firstMsg, const char* name,
                                       bool joinable) {
  pthread_mutex_lock(&LocContext::mGetLocContextMutex);
  LOC_LOGD("%s:%d]: querying ContextBase with tCreator", __func__, __LINE__);
  if (NULL == mContext) {
    LOC_LOGD("%s:%d]: creating msgTask with tCreator", __func__, __LINE__);
    const MsgTask* msgTask = getMsgTask(tCreator, name, joinable);
    mContext = new LocContext(msgTask);
  }
  pthread_mutex_unlock(&LocContext::mGetLocContextMutex);

  if (firstMsg) {
    mContext->sendMsg(firstMsg);
  }

  return mContext;
}

void LocContext ::injectFeatureConfig(ContextBase* curContext) {
  LOC_LOGD("%s:%d]: Calling LBSProxy (%p) to inject feature config", __func__,
           __LINE__, ((LocContext*)curContext)->mLBSProxy);
  ((LocContext*)curContext)->mLBSProxy->injectFeatureConfig(curContext);
}

LocContext::LocContext(const MsgTask* msgTask)
    : ContextBase(msgTask, 0, mLBSLibName) {}

}  // namespace loc_core
