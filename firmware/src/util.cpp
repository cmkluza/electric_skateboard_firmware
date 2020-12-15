/*
 * util.hpp - miscellaneous utilities.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "util.hpp"

#include <ble_advdata.h>
#include <bsp.h>
#include <bsp_btn_ble.h>
#include <nrf.h>
#include <nrf_drv_clock.h>
#include <nrf_soc.h>
#include <sdk_errors.h>

#include "logger.hpp"
using logger::Level;
using logger::Option;

namespace util {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Helper to call the log function with the no header option.
 *
 * @param[in] fmt  the string format for the log
 * @param[in] args arguments for the formatted log
 */
template <typename ... Args>
static inline void log_raw(const char *fmt, Args ... args);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void clock_init() {
    if (!nrf_drv_clock_init_check()) {
        nrf_drv_clock_init();
    }
}

void enable_deep_sleep() {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

void log_uuid(const ble_uuid128_t *uuid) {
    /* Log format: E44D8CF2-8112-44A6-B41C-73BA7EFA957C
     *    # bytes:     4   - 2  - 2  - 2  -    6
     * Bytes are stored in uuid structure backwards
     */
    log_raw("128-bit UUID: %02X%02X%02X%02X",
            uuid->uuid128[15], uuid->uuid128[14],
            uuid->uuid128[13], uuid->uuid128[12]);
    log_raw("-%02X%02X", uuid->uuid128[11], uuid->uuid128[10]);
    log_raw("-%02X%02X", uuid->uuid128[9], uuid->uuid128[8]);
    log_raw("-%02X%02X", uuid->uuid128[7], uuid->uuid128[6]);
    log_raw("-%02X%02X%02X%02X%02X%02X\n",
            uuid->uuid128[5], uuid->uuid128[4], uuid->uuid128[3],
            uuid->uuid128[2], uuid->uuid128[1], uuid->uuid128[0]);
}

void log_ble_data(const ble_data_t *data) {
    /* Log the device name */
    std::uint16_t offset { 0 };
    std::uint16_t len = ble_advdata_search(data->p_data, data->len, &offset,
                                           BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME);

    std::uint8_t *name_data = &data->p_data[offset];
    char name[31] = {};
    // BLE advertisement should always be smaller than this, but be sure
    len = len < 30 ? len : 30;
    memcpy(name, name_data, len);

    log_raw("Name: %s\n", logger::push(name));
    log_raw("Name: %s\n", logger::push(name));

    /* Log any 16-bit UUIDs */
    offset = {};
    len = ble_advdata_search(data->p_data, data->len, &offset,
                             BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE);
    if (0 == offset) {  // check for other UUIDs
        len = ble_advdata_search(data->p_data, data->len, &offset,
                                 BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE);
    }

    if (0 != offset) {
        std::uint8_t *uuid_data = &data->p_data[offset];
        for (std::uint16_t uuid_offset = 0; uuid_offset < len; uuid_offset += UUID16_LEN) {
            log_raw("16-bit UUID: 0x%02X%02X\n", uuid_data[uuid_offset + 1],
                     uuid_data[uuid_offset]);
        }
    }

    /* Log any 128-bit UUIDs */
    offset = {};
    len = ble_advdata_search(data->p_data, data->len, &offset,
                             BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE);
    if (0 == offset) {  // check for other UUIDs
        len = ble_advdata_search(data->p_data, data->len, &offset,
                                 BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE);
    }

    if (0 != offset) {
        std::uint8_t *uuid_data = &data->p_data[offset];
        ble_uuid128_t uuid;
        for (std::uint16_t uuid_offset = 0; uuid_offset < len; uuid_offset += UUID128_LEN) {
            memcpy(uuid.uuid128, &uuid_data[uuid_offset], UUID128_LEN);
            log_uuid(&uuid);
        }
    }

    /* Log MFG data */
    offset = {};
    len = ble_advdata_search(data->p_data, data->len, &offset,
                             BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA);
    if (0 != offset) {
        log_raw("MFG Data: ");
        logger::hexdump<Level::INFO>(&data->p_data[offset], len);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename ... Args>
static inline void log_raw(const char *fmt, Args ... args) {
    logger::log<Level::INFO, Option::NO_HEADER>(fmt, args...);
}

}  // namespace util
