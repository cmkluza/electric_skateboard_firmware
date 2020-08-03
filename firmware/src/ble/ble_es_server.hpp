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

#include "hall_sensor.hpp"

class BLEESServer {
 private:
    /**< Service handle for this service (provided by BLE stack). */
    std::uint16_t _service_handle {};
    /**< Handles for the sensor characteristic. */
    ble_gatts_char_handles_t _sensor_char_handles {};
    /**< Handle for the connection to the receiver. */
    std::uint16_t _conn_handle { BLE_CONN_HANDLE_INVALID };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
 public:
    /**< Macro to define a BLE event observer. */
    #define BLE_ES_SERVER_DEF(_name) \
        static BLEESServer _name; \
        NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                             BLE_ES_OBSERVER_PRIO, \
                             BLEESServer::event_handler, &_name)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
 public:
    /**
     * Initializes this service and its characteristics.
     */
    void init();

    /**
     * Update the sensor value.
     *
     * @param[in] new_value the new sensor value to send to the client.
     */
    void update_sensor_value(HallSensor::type new_value);

    /**
     * BLE event handler for this service.
     *
     * @param[in] p_ble_evt the BLE event.
     * @param[in] p_context context passed when this handler is registered (pointer to "this").
     */
    static void event_handler(ble_evt_t const *p_ble_evt, void *p_context);
};  // class BLEESServer
