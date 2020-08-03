/*
 * ble_es_common.hpp - Common definitions and routines for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <ble_types.h>

#include <cstdint>

namespace ble_es_common {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Randomly generated 128-bit UUID base for custom electric
     skateboard service:
     E44D8CF2-8112-44A6-B41C-73BA7EFA957C */
inline constexpr ble_uuid128_t UUID_BASE = {
    .uuid128 = { 0x7C, 0x95, 0xFA, 0x7E, 0xBA, 0x73, 0x1C, 0xB4,
                 0xA6, 0x44, 0x12, 0x81, 0xF2, 0x8C, 0x4D, 0xE4 }
};

/**< UUID for custom electric skateboard service.
     E44D0001-8112-44A6-B41C-73BA7EFA957C */
inline constexpr std::uint16_t UUID_SERVICE = { 0x0001 };
/**< UUID for Hall effect sensor data.
     E44D0002-8112-44A6-B41C-73BA7EFA957C */
inline constexpr std::uint16_t UUID_SENSOR_CHAR = { 0x0002 };
/**< Randomly generated appearance for the remote. */
inline constexpr std::uint16_t APPEARANCE = { 0xFA66 };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Initializes the vendor-specific UUID. */
void init();

/**< Returns the UUID type provided by the BLE stack. */
std::uint8_t uuid_type();

};  // namespace ble_es_common
