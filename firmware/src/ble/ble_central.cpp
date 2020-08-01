/*
 * ble_central.cpp - BLE central module.
 *
 * Performs initialization and maintenance of various BLE central functionalities.
 *
 * The electric skatebaord remote shall act primarily as a BLE central. It will find and connect to
 * the electric skateboard receiver, and serve sensor data to it so it can set speed accordingly.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_central.hpp"

#include <nrf_assert.h>
#include <nrf_ble_gatt.h>
#include <nrf_ble_gq.h>
#include <nrf_log.h>
#include <nrf_ble_scan.h>

namespace ble_central {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes BLE scanner with default parameters and an event handler.
 *
 * @param[in] handler pointer to the event handler to initialize the scan module with.
 */
static void scan_init(nrf_ble_scan_evt_handler_t handler);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< nRF BLE GATT instance. */
static nrf_ble_gatt_t *g_gatt;
/**< nRF BLE scanner instance. */
static nrf_ble_scan_t *g_scan;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init(const ble_common::Data &data) {
    g_gatt = data.gatt;
    g_scan = data.scan;

    ASSERT(g_scan != nullptr &&
           g_gatt != nullptr);

    ble_common::init(data);

    /* Initialize BLE central-specific modules */
    scan_init(data.scan_handler);
}

void set_addr_scan_filter(const ble_gap_addr_t &addr) {
    ASSERT(g_scan != nullptr);
    APP_ERROR_CHECK(nrf_ble_scan_filters_disable(g_scan));
    APP_ERROR_CHECK(nrf_ble_scan_all_filter_remove(g_scan));
    APP_ERROR_CHECK(nrf_ble_scan_filters_enable(g_scan, NRF_BLE_SCAN_ADDR_FILTER, true));

    /* SCAN_ADDR_FILTER takes a (uint8_t *) */
    nrf_ble_scan_filter_set(g_scan, SCAN_ADDR_FILTER, addr.addr);
}

void set_uuid_appearance_scan_filter(const ble_uuid_t &uuid, std::uint16_t appearance) {
    ASSERT(g_scan != nullptr);

    APP_ERROR_CHECK(nrf_ble_scan_filters_disable(g_scan));
    APP_ERROR_CHECK(nrf_ble_scan_all_filter_remove(g_scan));

    APP_ERROR_CHECK(nrf_ble_scan_filters_enable(g_scan,
                    (NRF_BLE_SCAN_UUID_FILTER | NRF_BLE_SCAN_APPEARANCE_FILTER), true));

    /* SCAN_UUID_FILTER takes a (ble_uuid_t *) */
    nrf_ble_scan_filter_set(g_scan, SCAN_UUID_FILTER, &uuid);
    /* SCAN_APPEARANCE_FILTER takes a (uint16_t *) */
    nrf_ble_scan_filter_set(g_scan, SCAN_APPEARANCE_FILTER, &appearance);
}

void begin_scanning() {
    ASSERT(g_scan != nullptr);
    APP_ERROR_CHECK(nrf_ble_scan_start(g_scan));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void scan_init(nrf_ble_scan_evt_handler_t handler) {
    nrf_ble_scan_init_t init_scan = {
        .p_scan_param = nullptr,   /** Use default scan parameters. */
        .connect_if_match = true,  /** Connect once a filter match is found. */
        .p_conn_param = nullptr,   /** Use default connection parameters. */
        .conn_cfg_tag = BLE_COMMON_CONN_CFG_TAG,
    };

    APP_ERROR_CHECK(nrf_ble_scan_init(g_scan, &init_scan, handler));
}

}  // namespace ble_central
