/*
 * ble_es_common.cpp - Common definitions and routines for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include "ble_es_common.hpp"

#include <app_error.h>
#include <ble.h>

std::uint8_t BLEESCommon::_uuid_type;

void BLEESCommon::init()
{
    /* Add vendor specific 128-bit UUID */
    ble_uuid128_t base_uuid = UUID_BASE;
    APP_ERROR_CHECK(sd_ble_uuid_vs_add(&base_uuid, &_uuid_type));
}
