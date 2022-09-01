/* Copyright (c) 2015-2017, 2020 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#ifndef __SYSTEM_STATUS_OSOBSERVER__
#define __SYSTEM_STATUS_OSOBSERVER__

#include <DataItemId.h>
#include <IOsObserver.h>
#include <LocUnorderedSetMap.h>
#include <MsgTask.h>
#include <loc_pla.h>
#include <log_util.h>

#include <cinttypes>
#include <list>
#include <map>
#include <new>
#include <string>
#include <vector>

namespace loc_core {
/******************************************************************************
 SystemStatusOsObserver
******************************************************************************/
using namespace std;
using namespace loc_util;

// Forward Declarations
class IDataItemCore;
class SystemStatus;
class SystemStatusOsObserver;
typedef map<IDataItemObserver*, list<DataItemId>> ObserverReqCache;
typedef LocUnorderedSetMap<IDataItemObserver*, DataItemId> ClientToDataItems;
typedef LocUnorderedSetMap<DataItemId, IDataItemObserver*> DataItemToClients;
typedef unordered_map<DataItemId, IDataItemCore*> DataItemIdToCore;
typedef unordered_map<DataItemId, int> DataItemIdToInt;
#ifdef USE_GLIB
// Cache details of backhaul client requests
typedef unordered_set<string> ClientBackhaulReqCache;
#endif

struct ObserverContext {
  IDataItemSubscription* mSubscriptionObj;
  IFrameworkActionReq* mFrameworkActionReqObj;
  const MsgTask* mMsgTask;
  SystemStatusOsObserver* mSSObserver;

  inline ObserverContext(const MsgTask* msgTask,
                         SystemStatusOsObserver* observer)
      : mSubscriptionObj(NULL),
        mFrameworkActionReqObj(NULL),
        mMsgTask(msgTask),
        mSSObserver(observer) {}
};

// Clients wanting to get data from OS/Framework would need to
// subscribe with OSObserver using IDataItemSubscription interface.
// Such clients would need to implement IDataItemObserver interface
// to receive data when it becomes available.
class SystemStatusOsObserver : public IOsObserver {
 public:
  // ctor
  inline SystemStatusOsObserver(SystemStatus* systemstatus,
                                const MsgTask* msgTask)
      : mSystemStatus(systemstatus),
        mContext(msgTask, this),
        mAddress("SystemStatusOsObserver"),
        mClientToDataItems(MAX_DATA_ITEM_ID),
        mDataItemToClients(MAX_DATA_ITEM_ID) {}

  // dtor
  ~SystemStatusOsObserver();

  template <typename CINT, typename COUT>
  static COUT containerTransfer(CINT& s);
  template <typename CINT, typename COUT>
  inline static COUT containerTransfer(CINT&& s) {
    return containerTransfer<CINT, COUT>(s);
  }

  // To set the subscription object
  virtual void setSubscriptionObj(IDataItemSubscription* subscriptionObj);

  // To set the framework action request object
  inline void setFrameworkActionReqObj(
      IFrameworkActionReq* frameworkActionReqObj) {
    mContext.mFrameworkActionReqObj = frameworkActionReqObj;
#ifdef USE_GLIB
    uint32_t numBackHaulClients = mBackHaulConnReqCache.size();
    if (numBackHaulClients > 0) {
      // For each client, invoke connectbackhaul.
      for (auto clientName : mBackHaulConnReqCache) {
        LOC_LOGd("Invoke connectBackhaul for client: %s", clientName.c_str());
        connectBackhaul(clientName);
      }
      // Clear the set
      mBackHaulConnReqCache.clear();
    }
#endif
  }

  // IDataItemSubscription Overrides
  inline virtual void subscribe(const list<DataItemId>& l,
                                IDataItemObserver* client) override {
    subscribe(l, client, false);
  }
  virtual void updateSubscription(const list<DataItemId>& l,
                                  IDataItemObserver* client) override;
  inline virtual void requestData(const list<DataItemId>& l,
                                  IDataItemObserver* client) override {
    subscribe(l, client, true);
  }
  virtual void unsubscribe(const list<DataItemId>& l,
                           IDataItemObserver* client) override;
  virtual void unsubscribeAll(IDataItemObserver* client) override;

  // IDataItemObserver Overrides
  virtual void notify(const list<IDataItemCore*>& dlist) override;
  inline virtual void getName(string& name) override { name = mAddress; }

  // IFrameworkActionReq Overrides
  virtual void turnOn(DataItemId dit, int timeOut = 0) override;
  virtual void turnOff(DataItemId dit) override;
#ifdef USE_GLIB
  virtual bool connectBackhaul(const string& clientName) override;
  virtual bool disconnectBackhaul(const string& clientName) override;
#endif

 private:
  SystemStatus* mSystemStatus;
  ObserverContext mContext;
  const string mAddress;
  ClientToDataItems mClientToDataItems;
  DataItemToClients mDataItemToClients;
  DataItemIdToCore mDataItemCache;
  DataItemIdToInt mActiveRequestCount;

  // Cache the subscribe and requestData till subscription obj is obtained
  void cacheObserverRequest(ObserverReqCache& reqCache,
                            const list<DataItemId>& l,
                            IDataItemObserver* client);
#ifdef USE_GLIB
  // Cache the framework action request for connect/disconnect
  ClientBackhaulReqCache mBackHaulConnReqCache;
#endif

  void subscribe(const list<DataItemId>& l, IDataItemObserver* client,
                 bool toRequestData);

  // Helpers
  void sendCachedDataItems(const unordered_set<DataItemId>& s,
                           IDataItemObserver* to);
  bool updateCache(IDataItemCore* d);
  inline void logMe(const unordered_set<DataItemId>& l) {
    IF_LOC_LOGD {
      for (auto id : l) {
        LOC_LOGD("DataItem %d", id);
      }
    }
  }
};

}  // namespace loc_core

#endif  //__SYSTEM_STATUS__
