/*
 * ble_es_client.hpp - Client for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

#include <cstdint>

/**< Callback for when sensor data comes in */
// TODO CMK 06/22/20: verify data type for sensor data
typedef void (*ble_es_sensor_callback)(uint8_t data);

class BLEESClient {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types, Constants, Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    uint16_t service_handle;                        /**< Handle for this service (provided by BLE stack) */
    ble_gatts_char_handles_t sensor_char_handles;   /**< Handles for the sensor characteristic */
    uint8_t uuid_type;                              /**< UUID for this service (provided by BLE stack) */
    ble_es_sensor_callback callback;              /**< Callback for when new sensor data comes in */

public:
    /** Macro to define a BLE event observer */
    // TODO CMK 07/08/20: replace observer priority
    #define BLE_ES_CLIENT_DEF(_name) \
    static BLEESClient _name; \
    NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                         BLE_LBS_BLE_OBSERVER_PRIO, \
                         BLEESClient::event_handler, &_name)

    /**< Randomly generated 128-bit UUID base for custom electric
         skateboard service:
         E44D8CF2-8112-44A6-B41C-73BA7EFA957C */
    static inline constexpr uint8_t UUID_BASE[] =
        { 0x7C, 0x95, 0xFA, 0x7E, 0xBA, 0x73, 0x1C, 0xB4,
          0xA6, 0x44, 0x12, 0x81, 0xF2, 0x8C, 0x4D, 0xE4 };

    /**< Randomly generated 16-bit UUID for custom electric skateboard service */
    static inline constexpr uint16_t UUID_SERVICE = { 0xDC10 };
    /**< 16-bit UUID for Hall effect sensor data */
    static inline constexpr uint16_t UUID_SENSOR_CHAR = { 0xDC11 };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
public:

    /**
     * Initializes this service.
     */
    void init();

    /**
     * BLE event handler for this service.
     */
    static void event_handler(ble_evt_t const *p_ble_evt, void *p_context);

}; // class BLEESClient
