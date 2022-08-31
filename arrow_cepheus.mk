#
# Copyright (C) 2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#
# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
# Inherit from cepheus device
$(call inherit-product, device/xiaomi/cepheus/device.mk)
# Inherit some common PE stuff.
$(call inherit-product, vendor/arrow/config/common_full_phone.mk)
# Boot animation
TARGET_BOOT_ANIMATION_RES := 1080
# Device identifier
PRODUCT_NAME := arrow_cepheus
PRODUCT_DEVICE := cepheus
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := Mi 9
PRODUCT_MANUFACTURER := Xiaomi
# Include firmware
$(call inherit-product, vendor/xiaomi-firmware/cepheus/firmware.mk)
TARGET_SUPPORTS_QUICK_TAP := true
