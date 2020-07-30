/*
 * ble_es_server.hpp - Server for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include "ble_es_common.hpp"

#include <app_config.h>
#include <ble_types.h>
#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

#include <cstdint>

class BLEESServer {
 private:
    /**< Service handle for this service (provided by BLE stack). */
    std::uint16_t _service_handle;
    /**< Handles for the sensor characteristic */
    ble_gatts_char_handles_t _sensor_char_handles;
    /**< Handle for the connection to the receiver */
    std::uint16_t _conn_handle;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Types, Constants, Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
 public:
    /**< Macro to define a BLE event observer */
    #define BLE_ES_SERVER_DEF(_name) \
        static BLEESServer _name; \
        NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                             BLE_ES_OBSERVER_PRIO, \
                             BLEESServer::event_handler, &_name)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
 public:
    /**
     * Initializes this service and its characteristics.
     */
    void init();

    // TODO(CMK) 06/22/20: verify sensor data type
    /**
     * Update the sensor value.
     *
     * @param TODO
     */
    void update_sensor_value(std::uint8_t new_value);

    /**
     * BLE event handler for this service.
     *
     * @param[in] p_ble_evt pointer to the event
     * @param[in] p_context pointer to self (passed when defined by BLE_ES_SERVER_DEF)
     */
    static void event_handler(ble_evt_t const *p_ble_evt, void *p_context);

 private:
////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and Destructors
////////////////////////////////////////////////////////////////////////////////////////////////////
    BLEESServer() = delete;
    ~BLEESServer() = delete;
};  // class BLEESServer
