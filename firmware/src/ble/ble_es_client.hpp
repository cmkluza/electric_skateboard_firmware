/*
 * ble_es_client.hpp - Client for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <ble_db_discovery.h>
#include <nrf_ble_gq.h>
#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

#include <cstdint>

// TODO CMK 07/27/20: delete constructors where applicable (e.g. here where global instances are defined)

class BLEESClient {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types, Constants, Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
    /**< Callback for when sensor data comes in */
    // TODO CMK 06/22/20: verify data type for sensor data
    using SensorCallback = void (*)(std::uint8_t);

private:
    /* Handles to relevant characteristics on the server */
    std::uint16_t _es_hall_handle;      /**< Handle to ES server's Hall sensor char. */
    std::uint16_t _es_hall_cccd_handle; /**< Handle to ES server's Hall sensor CCCD. */
    std::uint8_t _uuid_type;            /**< UUID for this service (provided by BLE stack). */
    std::uint16_t _conn_handle;         /**< Connection handle to the remote. */
    SensorCallback _callback;           /**< Callback for when new sensor data comes in. */
    nrf_ble_gq_t *_gatt_queue;          /**< Pointer to GATT queue instance. */
    
public:
    /** Macro to define a BLE event observer */
    // TODO CMK 07/08/20: replace observer priority
    #define BLE_ES_CLIENT_DEF(_name) \
    static BLEESClient _name; \
    NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                         BLE_ES_OBSERVER_PRIO, \
                         BLEESClient::event_handler, &_name)

    /**< Randomly generated 128-bit UUID base for custom electric 
         skateboard service:
         E44D8CF2-8112-44A6-B41C-73BA7EFA957C */
    static inline constexpr ble_uuid128_t UUID_BASE = { 
        .uuid128 = { 0x7C, 0x95, 0xFA, 0x7E, 0xBA, 0x73, 0x1C, 0xB4,
                     0xA6, 0x44, 0x12, 0x81, 0xF2, 0x8C, 0x4D, 0xE4 } 
    };

    /**< UUID for custom electric skateboard service. 
         E44D0001-8112-44A6-B41C-73BA7EFA957C */
    static inline constexpr std::uint16_t UUID_SERVICE = { 0x0001 };
    /**< UUID for Hall effect sensor data. 
         E44D0002-8112-44A6-B41C-73BA7EFA957C */    
    static inline constexpr std::uint16_t UUID_SENSOR_CHAR = { 0x0002 };
    /**< BLE appearance for the remote. */
    // TODO CMK 07/01/20: see if there's regulations/considerations for custom appearances
    static inline constexpr std::uint16_t APPEARANCE = { 0x1234 };

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
     * Gets the BLE UUID type assigned by the Nordic BLE stack.
     */
    std::uint8_t uuid_type() { return _uuid_type; }
    
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
