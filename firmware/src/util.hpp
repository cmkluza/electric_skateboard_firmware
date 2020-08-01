/*
 * util.hpp - miscellaneous utilities.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <ble_types.h>

#include <cstdint>

#include "config/app_config.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Macros for printing 6-byte MAC addresses.
 */
#define MAC_FMT "%02X:%02X:%02X:%02X:%02X:%02X"
#define MAC_ARGS(mac_byte_ptr) \
    (mac_byte_ptr)[5], (mac_byte_ptr)[4], (mac_byte_ptr)[3], \
    (mac_byte_ptr)[2], (mac_byte_ptr)[1], (mac_byte_ptr)[0]

/**
 * Macros for supressing single warnings.
 */
#ifdef __GNUC__
#   define DO_PRAGMA(x) _Pragma (#x)
#   define SUPPRESS_WARNING_START(warning) \
        _Pragma("GCC diagnostic push") \
        DO_PRAGMA(GCC diagnostic ignored warning)
#   define SUPPRESS_WARNING_END() \
        _Pragma("GCC diagnostic pop")
#else
#   define SUPPRESS_WARNING_START(warning)
#   define SUPRESS_WARNING_END()
#endif

namespace util {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////////////////////////

    /**< Length of a 16-bit UUID. */
    inline constexpr std::uint32_t UUID16_LEN = { 2 };

    /**< Length of a 128-bit UUID. */
    inline constexpr std::uint32_t UUID128_LEN = { 16 };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * Function for initializing the clock.
     */
    void clock_init();

    /**
     * Enables deep sleep instead of regular sleep in the SCB.
     */
    void enable_deep_sleep();

    /**
     * Logs a 128-bit UUID.
     *
     * @param[in] uuid pointer to the UUID to be logged.
     */
    void log_uuid(const ble_uuid128_t *uuid);

    /**
     * Logs a piece of advertisement data.
     *
     * @param[in] data pointer to the advertisement data.
     */
    void log_ble_data(const ble_data_t *data);
}  // namespace util
