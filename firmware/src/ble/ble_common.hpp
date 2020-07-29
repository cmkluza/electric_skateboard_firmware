/*
 * ble_common.hpp - Common BLE functions.
 *
 * Contains common BLE functions, such as setting up the BLE stack.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include "ble_es_client.hpp"
#include "ble_es_server.hpp"
#include "util.hpp"

#include <ble_advertising.h>
#include <ble_db_discovery.h>
#include <nrf_ble_gatt.h>
#include <nrf_ble_gq.h>
#include <nrf_ble_scan.h>
#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

namespace ble_common {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////

SUPPRESS_WARNING_START("-Wsubobject-linkage")

    struct Config {
        /**< Common BLE observer instance. */
        nrf_sdh_ble_evt_observer_t *ble_observer;
        /**< nRF BLE GATT instance. */
        nrf_ble_gatt_t *gatt;
    
        union {
            struct {
                /**< nRF BLE scanner instance. */
                nrf_ble_scan_t *scan;
                /**< Scan event handler. */
                nrf_ble_scan_evt_handler_t scan_handler;
                /**< nRF GATT queue instance. */
                nrf_ble_gq_t *gatt_queue;
                /**< nRF BLE discovery instance. */
                ble_db_discovery_t *discovery;
            } central_config;
            struct {
                /**< nRF advertiser instance. */
                ble_advertising_t *advertising;
            } peripheral_config;
        };
    
        enum class ConfigType {
            CENTRAL,
            PERIPHERAL,
        } type;
    };

SUPPRESS_WARNING_END()

    /**
     * Initializes common BLE stack and modules.
     *
     * TODO CMK 06/24/20: specifics
     */
    void init(const Config &config);
    
    /**
     * Callback to handle BLE events.
     */
    void event_handler(ble_evt_t const *p_ble_evt, void *p_context);
        
} // namespace ble_common
