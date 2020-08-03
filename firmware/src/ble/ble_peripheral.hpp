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

#include <ble_gap.h>
#include <bsp.h>
#include <sdk_errors.h>

#include "ble_common.hpp"

namespace ble_peripheral {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Performs common BLE initialization and initializes GAP, connection parameters, advertising,
 * and DB discovery.
 *
 * @param[in] data struct containing pointers to instances of Nordic BLE modules.
 */
void init(const ble_common::Data &data);

/**
 * Sets advertising data to advertise this device's name.
 */
void advertise_name();

/**
 * Sets advertising data to advertise the ES service UUID and appearance.
 *
 * @param[in] uuid pointer to the BLE UUID to advertise.
 */
void advertise_uuid_appearance(ble_uuid_t *uuid);

/**
  * Starts advertising.
  */
void start_advertising();

}  // namespace ble_peripheral
