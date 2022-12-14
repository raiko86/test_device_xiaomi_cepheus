/* Copyright (c) 2017-2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#ifndef XTRA_SYSTEM_STATUS_OBS_H
#define XTRA_SYSTEM_STATUS_OBS_H

#include <LocIpc.h>
#include <LocTimer.h>
#include <MsgTask.h>
#include <stdlib.h>

#include <cinttypes>

using namespace std;
using namespace loc_util;
using loc_core::IDataItemCore;
using loc_core::IDataItemObserver;
using loc_core::IOsObserver;

struct StartDgnssNtripParams {
  GnssNtripConnectionParams ntripParams;
  string nmea;

  void clear() {
    ntripParams.hostNameOrIp.clear();
    ntripParams.mountPoint.clear();
    ntripParams.username.clear();
    ntripParams.password.clear();
    ntripParams.port = 0;
    ntripParams.useSSL = false;
    ntripParams.requiresNmeaLocation = false;
    nmea.clear();
  }
};

class XtraSystemStatusObserver : public IDataItemObserver {
 public:
  // constructor & destructor
  XtraSystemStatusObserver(IOsObserver* sysStatObs, const MsgTask* msgTask);
  inline virtual ~XtraSystemStatusObserver() {
    subscribe(false);
    mIpc.stopNonBlockingListening();
  }

  // IDataItemObserver overrides
  inline virtual void getName(string& name);
  virtual void notify(const list<IDataItemCore*>& dlist);

  bool updateLockStatus(GnssConfigGpsLock lock);
  bool updateConnections(uint64_t allConnections,
                         loc_core::NetworkInfoType* networkHandleInfo);
  bool updateTac(const string& tac);
  bool updateMccMnc(const string& mccmnc);
  bool updateXtraThrottle(const bool enabled);
  inline const MsgTask* getMsgTask() { return mMsgTask; }
  void subscribe(bool yes);
  bool onStatusRequested(int32_t xtraStatusUpdated);
  void startDgnssSource(const StartDgnssNtripParams& params);
  void restartDgnssSource();
  void stopDgnssSource();
  void updateNmeaToDgnssServer(const string& nmea);

 private:
  IOsObserver* mSystemStatusObsrvr;
  const MsgTask* mMsgTask;
  GnssConfigGpsLock mGpsLock;
  LocIpc mIpc;
  uint64_t mConnections;
  loc_core::NetworkInfoType mNetworkHandle[MAX_NETWORK_HANDLES];
  string mTac;
  string mMccmnc;
  bool mXtraThrottle;
  bool mReqStatusReceived;
  bool mIsConnectivityStatusKnown;
  shared_ptr<LocIpcSender> mSender;
  string mNtripParamsString;

  class DelayLocTimer : public LocTimer {
    LocIpcSender& mSender;

   public:
    DelayLocTimer(LocIpcSender& sender) : mSender(sender) {}
    void timeOutCallback() override {
      LocIpc::send(mSender, (const uint8_t*)"halinit", sizeof("halinit"));
    }
  } mDelayLocTimer;
};

#endif
