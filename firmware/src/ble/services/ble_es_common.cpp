/*
 * ble_es_common.cpp - Common definitions and routines for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_es_common.hpp"

#include <app_error.h>
#include <ble.h>

namespace ble_es_common {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

static std::uint8_t g_uuid_type;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init() {
    ble_uuid128_t base_uuid = UUID_BASE;
    APP_ERROR_CHECK(sd_ble_uuid_vs_add(&base_uuid, &g_uuid_type));
}

std::uint8_t uuid_type() {
    return g_uuid_type;
}

}  // namespace ble_es_common
