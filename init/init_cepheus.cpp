/*
   Copyright (C) 2020 The LineageOS Project.
   SPDX-License-Identifier: Linux-OpenIB
 */

#include <android-base/properties.h>
#include <unistd.h>

#include <fstream>
#include <vector>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"

using android::base::GetProperty;
using android::base::SetProperty;

std::vector<std::string> ro_props_default_source_order = {
    "", "bootimage.", "odm.", "product.", "system.", "vendor.",
};

void property_override(char const prop[], char const value[], bool add = true) {
  prop_info* pi;
  pi = (prop_info*)__system_property_find(prop);
  if (pi)
    __system_property_update(pi, value, strlen(value));
  else if (add)
    __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_ro_build_prop(const std::string& prop, const std::string& value) {
  for (const auto& source : ro_props_default_source_order) {
    auto prop_name = "ro." + source + "build." + prop;
    if (source == "")
      property_override(prop_name.c_str(), value.c_str());
    else
      property_override(prop_name.c_str(), value.c_str(), false);
  }
};

void set_ro_product_prop(const std::string& prop, const std::string& value) {
  for (const auto& source : ro_props_default_source_order) {
    auto prop_name = "ro.product." + source + prop;
    property_override(prop_name.c_str(), value.c_str(), false);
  }
};

void vendor_load_properties() {
  set_ro_product_prop("device", "cepheus");
  set_ro_product_prop("model", "MI 9");
  set_ro_product_prop("name", "cepheus");
  set_ro_build_prop(
      "fingerprint",
      "google/redfin/redfin:12/SQ1A.220105.002/7961164:user/release-keys");

  // description
  property_override("ro.build.description",
                    "redfin-user 12 SQ1A.220105.002 7961164 release-keys");

  // APEX
  property_override("ro.apex.updatable", "false");
}