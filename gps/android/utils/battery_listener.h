/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 */
typedef void (*battery_status_change_fn_t)(bool);
void loc_extn_battery_properties_listener_init(battery_status_change_fn_t fn);
void loc_extn_battery_properties_listener_deinit();
bool loc_extn_battery_properties_is_charging();
