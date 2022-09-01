/* Copyright (c) 2013-2015, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */
#ifndef IZAT_PROXY_BASE_H
#define IZAT_PROXY_BASE_H
#include <gps_extended.h>

namespace loc_core {

class LocApiBase;
class LocAdapterBase;
class ContextBase;

class LBSProxyBase {
  friend class ContextBase;
  inline virtual LocApiBase* getLocApi(LOC_API_ADAPTER_EVENT_MASK_T exMask,
                                       ContextBase* context) const {
    (void)exMask;
    (void)context;
    return NULL;
  }

 protected:
  inline LBSProxyBase() {}

 public:
  inline virtual ~LBSProxyBase() {}
  inline virtual bool hasAgpsExtendedCapabilities() const { return false; }
  inline virtual void modemPowerVote(bool power) const { (void)power; }
  virtual void injectFeatureConfig(ContextBase* context) const {
    (void)context;
  }
  inline virtual bool hasNativeXtraClient() const { return false; }
  inline virtual IzatDevId_t getIzatDevId() const { return 0; }
};

typedef LBSProxyBase*(getLBSProxy_t)();

}  // namespace loc_core

#endif  // IZAT_PROXY_BASE_H
