/* Copyright (c) 2014, 2016-2017 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef LOC_ADAPTER_PROXY_BASE_H
#define LOC_ADAPTER_PROXY_BASE_H

#include <ContextBase.h>
#include <gps_extended.h>

namespace loc_core {

class LocAdapterProxyBase {
 private:
  LocAdapterBase *mLocAdapterBase;

 protected:
  inline LocAdapterProxyBase(const LOC_API_ADAPTER_EVENT_MASK_T mask,
                             ContextBase *context, bool isMaster = false)
      : mLocAdapterBase(new LocAdapterBase(mask, context, isMaster, this)) {}
  inline virtual ~LocAdapterProxyBase() { delete mLocAdapterBase; }
  inline void updateEvtMask(LOC_API_ADAPTER_EVENT_MASK_T event,
                            loc_registration_mask_status isEnabled) {
    mLocAdapterBase->updateEvtMask(event, isEnabled);
  }

  inline uint32_t generateSessionId() {
    return mLocAdapterBase->generateSessionId();
  }

 public:
  inline ContextBase *getContext() const {
    return mLocAdapterBase->getContext();
  }

  inline virtual void handleEngineUpEvent(){};
  inline virtual void handleEngineDownEvent(){};
  inline virtual void reportPositionEvent(UlpLocation &location,
                                          GpsLocationExtended &locationExtended,
                                          enum loc_sess_status status,
                                          LocPosTechMask loc_technology_mask) {
    (void)location;
    (void)locationExtended;
    (void)status;
    (void)loc_technology_mask;
  }
};

}  // namespace loc_core

#endif  // LOC_ADAPTER_PROXY_BASE_H
