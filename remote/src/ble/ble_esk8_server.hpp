/*
 * ble_esk8_server.hpp - Server for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <app_config.h>
#include <ble_types.h>
#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

class BLEEsk8Server {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types, Constants, Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    uint16_t _service_handle;                       /**< Handle for this service (provided by BLE stack) */
    ble_gatts_char_handles_t _sensor_char_handles;  /**< Handles for the sensor characteristic */
    uint8_t _uuid_type;                             /**< UUID for this service (provided by BLE stack) */
    
public:
    /**< Macro to define a BLE event observer */
    #define BLE_ESK8_SERVER_DEF(_name) \
        static BLEEsk8Server _name; \
        NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                             BLE_ESK8_OBSERVER_PRIO, \
                             BLEEsk8Server::event_handler, &_name)

    /**< Randomly generated 128-bit UUID base for custom electric 
         skateboard service:
         E44D8CF2-8112-44A6-B41C-73BA7EFA957C */
    static inline constexpr ble_uuid128_t UUID_BASE = { 
        .uuid128 = { 0x7C, 0x95, 0xFA, 0x7E, 0xBA, 0x73, 0x1C, 0xB4,
                     0xA6, 0x44, 0x12, 0x81, 0xF2, 0x8C, 0x4D, 0xE4 } 
    };
    
    /**< Randomly generated 16-bit UUID for custom electric skateboard service */
    static inline constexpr uint16_t UUID_SERVICE = { 0xDC10 };
    /**< 16-bit UUID for Hall effect sensor data */    
    static inline constexpr uint16_t UUID_SENSOR_CHAR = { 0xDC11 };
            
////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////    
public:
    
    /**
     * Initializes this service and its characteristics.
     */
    void init();

    /**
     * BLE event handler for this service.
     *
     * @param[in] p_ble_evt pointer to the event
     * @param[in] p_context pointer to self (passed when defined by BLE_ESK8_CLIENT_DEF)
     */
    static void event_handler(ble_evt_t const *p_ble_evt, void *p_context);
    
    // TODO CMK 06/22/20: verify sensor data type
    /**
     * Update the sensor value.
     *
     * @param TODO
     */
    void update_sensor_value(uint16_t conn_handle, uint8_t new_value);

}; // class BLEEsk8Server
