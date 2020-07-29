/*
 * ble_es_client.hpp - Client for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include "ble_es_common.hpp"

#include <ble_db_discovery.h>
#include <nrf_ble_gq.h>
#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

#include <cstdint>

// TODO CMK 07/27/20: delete constructors where applicable (e.g. here where global instances are defined)

class BLEESClient {
    /**< Callback for when sensor data comes in */
    // TODO CMK 06/22/20: verify data type for sensor data
    using SensorCallback = void (*)(std::uint8_t);

private:
    /* Handles to relevant characteristics on the server */
    std::uint16_t _es_hall_handle;      /**< Handle to ES server's Hall sensor char. */
    std::uint16_t _es_hall_cccd_handle; /**< Handle to ES server's Hall sensor CCCD. */
    std::uint16_t _conn_handle;         /**< Connection handle to the remote. */
    SensorCallback _callback;           /**< Callback for when new sensor data comes in. */
    nrf_ble_gq_t *_gatt_queue;          /**< Pointer to GATT queue instance. */
    
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types, Constants, Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
    /** Macro to define a BLE event observer */
    #define BLE_ES_CLIENT_DEF(_name) \
    static BLEESClient _name; \
    NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                         BLE_ES_OBSERVER_PRIO, \
                         BLEESClient::event_handler, &_name)


////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
public:

    /**
     * Initializes this service.
     */
    void init(nrf_ble_gq_t *gatt_queue);
    
    // TODO CMK 07/27/20: subscribe and handle notifications
    /**
     * Register an application callback for sensor data notifications.
     */
    void register_sensor_data_callback(SensorCallback callback)
    {
        _callback = callback;
    }

    /**
     * Callback for DB discovery events.
     */
    void on_db_discovery_evt(const ble_db_discovery_evt_t *p_evt);

    /**
     * BLE event handler for this service.
     */
    static void event_handler(ble_evt_t const *p_ble_evt, void *p_context);
    
private:
    /**
     * Subscribes to notifications from the remote upon DB discovery completion.
     */
    void subscribe_to_notifications();
}; // class BLEESClient