/*
 * ble_remote.hpp - Electric skateboard remote main BLE wrapper.
 *
 * The primary module for the electric skateboard remote's BLE functionality.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstdint>

namespace ble_remote {
    
    /**
     * Initializes the BLE stack and starts scanning for a receiver.
     */
    void init();
    
    /**
     *
     */
    void update_sensor_value(std::uint8_t value);
    
}; // namespace ble_remote
