/*
 * ble_peripheral.hpp - BLE peripheral module.
 *
 * Performs initialization and maintenance of various BLE peripheral functionalities.
 *
 * The electric skateboard receiver shall act solely as a BLE peripheral, subscribing to
 * a characteristic that will give it sensor data. From this it can determine what the current
 * speed should be.
 *
 * The electric skatebaord remote shall eventually have a secondary BLE peripheral functionality.
 * It will connect to a mobile phone to get useful info that might be displayed on the remote,
 * such as date-time, directions, etc.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include "ble_common.hpp"

#include <ble_gap.h>
#include <bsp.h>
#include <sdk_errors.h>

namespace ble_peripheral {
    /**
     * Initializes BLE peripheral modules.
     *
     * TODO(CMK) 06/24/20: update with specifics
     */
    void init(const ble_common::Config &config);

    /**
     * Sets advertising data to advertise this device's name.
     */
    void advertise_name();

    /**
     * Sets advertising data to advertise the ES service UUID and appearance.
     */
    void advertise_uuid_appearance(const ble_uuid_t &uuid);

    /**
      * Starts advertising.
      */
    void start_advertising();
}  // namespace ble_peripheral
