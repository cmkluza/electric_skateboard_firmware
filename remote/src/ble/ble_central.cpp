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

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ble_central.hpp"

#include <ble_db_discovery.h>
#include <nrf_assert.h>
#include <nrf_ble_gatt.h>
#include <nrf_ble_gq.h>
#include <nrf_ble_scan.h>

namespace ble_central {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Data
////////////////////////////////////////////////////////////////////////////////////////////////////

static nrf_ble_gatt_t *g_gatt; /**< nRF BLE GATT instance. */
static nrf_ble_scan_t *g_scan; /**< nRF BLE scanner instance. */
static nrf_ble_gq_t *g_gatt_queue; /**< nRF GATT queue instance. */
static ble_db_discovery_t *g_discovery; /**< nRF BLE discovery instance. */

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes BLE scanner with default parameters and the given event handler.
 */
static void scan_init(nrf_ble_scan_evt_handler_t event_handler);

/** TODO CMK 06/29/20: document */
static void db_discovery_init();

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

ret_code_t init(const ble_common::Config &config)
{
    /* Preconditions */
    ASSERT(config.type == ble_common::Config::ConfigType::CENTRAL);
    
    g_gatt = config.gatt;
    g_scan = config.central_config.scan;
    g_gatt_queue = config.central_config.gatt_queue;
    g_discovery = config.central_config.discovery;
    
    ASSERT(g_scan != nullptr &&
           g_gatt != nullptr &&
           g_gatt_queue != nullptr &&
           g_discovery != nullptr);
    
    /* Ensure common BLE init takes place first */
    bool common_is_initialized = *static_cast<bool *>(config.ble_observer->p_context);
    
    if (!common_is_initialized) {
        ble_common::init(config);
    }

    /* Initialize BLE central-specific modules */
    scan_init(config.central_config.scan_handler);
    db_discovery_init();
}

void set_scan_filter(uint8_t filter_mode)
{
    ASSERT(g_scan != nullptr);
    err_code = nrf_ble_scan_filters_enable(g_scan, filter_mode, true);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_scan_filter_set(g_scan, SCAN_NAME_FILTER, m_target_periph_name);
}

void begin_scanning()
{
    ASSERT(g_scan != nullptr);
    APP_ERROR_CHECK(nrf_ble_scan_start(g_scan));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void scan_init(nrf_ble_scan_evt_handler_t event_handler)
{
    nrf_ble_scan_init_t init = {
        .p_scan_param = nullptr, /** Use default scan parameters. */
        .connect_if_match = false, /** Let app handle potential connections. */
        .p_conn_param = nullptr, /** Use default connection parameters. */
        .conn_cfg_tag = BLE_COMMON_CONN_CFG_TAG,
    };
    
    APP_ERROR_CHECK(nrf_ble_scan_init(g_scan, &init, event_handler));
}

static void db_discovery_init()
{
    ble_db_discovery_init_t db_init;

    // TODO CMK 06/25/20: db event handler
    db_init.evt_handler  = nullptr;
    db_init.p_gatt_queue = g_gatt_queue;

    APP_ERROR_CHECK(ble_db_discovery_init(&db_init));
}

} // namespace ble_central