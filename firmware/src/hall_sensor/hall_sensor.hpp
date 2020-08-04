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

    /**
     * Converts a sensor type to a 2-byte buffer.
     *
     * @param[in] value the sensor value to convert.
     * @param[in] buffer the buffer to write into.
     */
    static inline void to_bytes(type value, std::uint8_t *buffer) {
        buffer[0] = (value >> 0) & 0xFF;
        buffer[1] = (value >> 8) & 0xFF;
    }

    /**
     * Converts a byte buffer into a sensor reading value.
     *
     * @param[in] buffer the byte buffer to convert from.
     * @return    the converted value.
     */
    static inline type from_bytes(const std::uint8_t *buffer) {
        return static_cast<type>(buffer[0] | (buffer[1] << 8));
    }
};
