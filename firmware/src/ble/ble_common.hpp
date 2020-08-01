/*
 * ble_common.hpp - Common BLE functions.
 *
 * Contains common BLE functions, such as setting up the BLE stack.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <ble_advertising.h>
#include <ble_db_discovery.h>
#include <nrf_ble_gatt.h>
#include <nrf_ble_gq.h>
#include <nrf_ble_scan.h>
#include <nrf_sdh_ble.h>
#include <sdk_errors.h>

#include "util.hpp"

namespace ble_common {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////

SUPPRESS_WARNING_START("-Wsubobject-linkage")

struct Data {
    /**< nRF BLE GATT instance. */
    nrf_ble_gatt_t *gatt;

    /**< nRF GATT queue instance. */
    nrf_ble_gq_t *gatt_queue;

    /**< nRF BLE scanner instance (central only). */
    nrf_ble_scan_t *scan;
    /**< Scan event handler (central only). */
    nrf_ble_scan_evt_handler_t scan_handler;

    /**< nRF advertiser instance (peripheral only). */
    ble_advertising_t *advertising;

    /**< nRF BLE discovery instance (peripheral only). */
    ble_db_discovery_t *discovery;
    /**< DB discovery event handler (peripheral only). */
    ble_db_discovery_evt_handler_t db_discovery_handler;
};

SUPPRESS_WARNING_END()

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes soft device and common BLE modules (i.e. GATT and peer manager).
 *
 * @param[in] data struct containing pointers to instances of Nordic BLE modules.
 */
void init(const Data &data);

}  // namespace ble_common
