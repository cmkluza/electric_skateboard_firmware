/*
 * hall_sensor.cpp - Drives readings from the analog Hall effect sensor.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "hall_sensor.hpp"

#include <sensorsim.h>

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Constants
////////////////////////////////////////////////////////////////////////////////////////////////////
/** Approximate min/max voltage outputs from the sensor. */
static constexpr auto SENSOR_MIN_V = 0.2;
static constexpr auto SENSOR_MAX_V = 2.0;
/** Reference voltage. */
static constexpr auto SENSOR_REF_V = 3.3;
/** Number of bits used per measurement. */
static constexpr auto MEASUREMENT_BITS = 12;
/** Minimum value the sensor could return. */
static constexpr std::uint32_t SENSOR_MIN_VAL =
    static_cast<std::uint32_t>((SENSOR_MIN_V / SENSOR_REF_V) * (1 << MEASUREMENT_BITS));
/** Maximum value the sensor could return. */
static constexpr std::uint32_t SENSOR_MAX_VAL =
    static_cast<std::uint32_t>((SENSOR_MAX_V / SENSOR_REF_V) * (1 << MEASUREMENT_BITS));

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

static sensorsim_cfg_t sim_cfg;
static sensorsim_state_t sim_state;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void HallSensor::init() {
    sim_cfg.min          = SENSOR_MIN_VAL;
    sim_cfg.max          = SENSOR_MAX_VAL;
    sim_cfg.incr         = 0x10;
    sim_cfg.start_at_max = true;

    sensorsim_init(&sim_state, &sim_cfg);
}

HallSensor::type HallSensor::read() {
    HallSensor::type sensor_value = sensorsim_measure(&sim_state, &sim_cfg);
    return sensor_value;
}
