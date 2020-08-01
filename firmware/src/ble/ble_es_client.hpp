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

class BLEESClient {
    /**< Callback for when sensor data comes in. */
    // TODO(CMK) 06/22/20: verify data type for sensor data
    using SensorCallback = void (*)(std::uint8_t);

 private:
    /**< Handle to ES server's Hall sensor char. */
    std::uint16_t _es_hall_handle;
    /**< Handle to ES server's Hall sensor CCCD. */
    std::uint16_t _es_hall_cccd_handle;
    /**< Connection handle to the remote. */
    std::uint16_t _conn_handle;
    /**< Callback for when new sensor data comes in. */
    SensorCallback _callback;
    /**< Pointer to GATT queue instance. */
    nrf_ble_gq_t *_gatt_queue;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
 public:
    /** Macro to define a BLE event observer. */
    #define BLE_ES_CLIENT_DEF(_name) \
    static BLEESClient _name; \
    NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                         BLE_ES_OBSERVER_PRIO, \
                         BLEESClient::event_handler, &_name)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////////////////////////////////////////////
    BLEESClient() : _es_hall_handle { },
                    _es_hall_cccd_handle { },
                    _conn_handle { BLE_CONN_HANDLE_INVALID },
                    _callback { },
                    _gatt_queue { } {}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
 public:
    /**
     * Initializes this service.
     *
     * @param[in] gatt_queue pointer to a GATT queue instance used by this module.
     */
    void init(nrf_ble_gq_t *gatt_queue);

    /**
     * Register an application callback for sensor data notifications.
     *
     * @param[in] callback the function to be called when sensor data is received.
     */
    void register_sensor_data_callback(SensorCallback callback) {
        _callback = callback;
    }

    /**
     * Callback for DB discovery events.
     *
     * @param[in] p_evt the DB discovery event.
     */
    void on_db_discovery_evt(const ble_db_discovery_evt_t *p_evt);

    /**
     * BLE event handler for this service.
     *
     * @param[in] p_ble_evt the BLE event.
     * @param[in] p_context context passed when this handler is registered (pointer to "this").
     */
    static void event_handler(ble_evt_t const *p_ble_evt, void *p_context);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
 private:
    /**
     * Subscribes to notifications from the ES server upon DB discovery completion.
     */
    void subscribe_to_notifications();
};  // class BLEESClient
