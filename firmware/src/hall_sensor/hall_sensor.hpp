/*
 * hall_sensor.hpp - Drives readings from the analog Hall effect sensor.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstdint>

struct HallSensor {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Return type of the Hall sensor ADC reading. */
    using type = std::uint16_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

    /** Initializes the ADC to read the analog Hall effect sensor. */
    void init();

    /**
     * Returns the most recently read value from the ADC connected to the Hall effect sensor.
     *
     * @return the most recently read sample.
     */
    type read();
};
