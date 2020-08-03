/*
 * ble_receiver.hpp - Electric skateboard receiver main BLE wrapper.
 *
 * The primary module for the electric skateboard remotereceivers's BLE functionality.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstdint>

#include "hall_sensor.hpp"

namespace ble_receiver {

using SensorCallback = void (*)(HallSensor::type);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the BLE stack and starts advertising.
 *
 * @param[in] sensor_callback a function to be called whenever new sensor data comes in.
 */
void init(SensorCallback sensor_callback);

}  // namespace ble_receiver
