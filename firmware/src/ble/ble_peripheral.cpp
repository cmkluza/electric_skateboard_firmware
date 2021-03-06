/*
 * ble_peripheral.cpp - BLE peripheral module.
 *
 * Performs initialization and maintenance of various BLE peripheral functionalities.
 *
 * The electric skateboard receiver shall act solely as a BLE peripheral, subscribing to
 * a characteristic that will give it sensor data. From this it can determine what the current
 * speed should be.
 *
 * The electric skatebaord remote shall eventually have a secondary BLE peripheral functionality.
 * It will connect to a mobile phone to get useful info that might be displayed on the remote,
 * such as date-time, directions, etc.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_peripheral.hpp"

#include <app_error.h>
#include <ble_advertising.h>
#include <ble_conn_params.h>
#include <ble_dis.h>
// TODO(CMK) 06/21/20: implement battery service client
#include <ble_bas_c.h>
// TODO(CMK) 06/21/20: implement current time client for phone connection?
// #include <ble_cts_c.h>
#include <bsp.h>
#include <nrf_ble_gatt.h>
#include <nrf_sdh.h>
#include <nrf_sdh_ble.h>

#include "ble_es_common.hpp"
#include "config/app_config.h"
#include "logger.hpp"
#include "util.hpp"

using logger::Level;

namespace ble_peripheral {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Configures GAP parameters.
 *
 * Sets up the security mode, appearance, and connection parameters for this device.
 */
static void gap_init();

/** Configures and initializes BLE advertising. */
static void advertising_init();

/** Configures and initializes connection parameters. */
static void conn_params_init();

/**
 * Configures and initializes the DB discovery module.
 *
 * @param[in] handler pointer to the event handler to initialize the DB discovery module with.
 */
static void db_discovery_init(ble_db_discovery_evt_handler_t handler);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< nRF BLE GATT instance. */
static nrf_ble_gatt_t *g_gatt;
/**< nRF BLE advertising instance. */
static ble_advertising_t *g_advertising;
/**< nRF DB discovery instance. */
static ble_db_discovery_t *g_discovery;
/**< nRF GATT queue instance. */
static nrf_ble_gq_t *g_gatt_queue;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init(const ble_common::Data &data) {
    g_gatt          = data.gatt;
    g_gatt_queue    = data.gatt_queue;
    g_advertising   = data.advertising;
    g_discovery     = data.discovery;

    ASSERT(g_gatt != nullptr &&
           g_gatt_queue != nullptr &&
           g_advertising != nullptr &&
           g_discovery != nullptr);

    ble_common::init(data);

    /* Initialize BLE peripheral-specific modules */
    gap_init();
    advertising_init();
    conn_params_init();
    db_discovery_init(data.db_discovery_handler);
}

void advertise_name() {
    ble_advdata_t advdata = {
        .name_type = BLE_ADVDATA_FULL_NAME,
    };

    APP_ERROR_CHECK(ble_advertising_advdata_update(g_advertising, &advdata, nullptr));

    logger::log<Level::INFO>("advertising name");
}

void advertise_uuid_appearance(ble_uuid_t *uuid) {
    ble_advdata_t advdata = {
        .include_appearance = true,
        .uuids_complete = {
            .uuid_cnt = 1,
            .p_uuids = uuid,
        },
    };

    APP_ERROR_CHECK(ble_advertising_advdata_update(g_advertising, &advdata, nullptr));

    logger::log<Level::INFO>("advertising UUID + appearance");
}

void start_advertising() {
    APP_ERROR_CHECK(ble_advertising_start(g_advertising, BLE_ADV_MODE_FAST));

    logger::log<Level::INFO>("%s", __func__);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void gap_init() {
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    APP_ERROR_CHECK(
        sd_ble_gap_device_name_set(&sec_mode,
           reinterpret_cast<const std::uint8_t *>(BLE_PERIPHERAL_DEVICE_NAME),
           strlen(BLE_PERIPHERAL_DEVICE_NAME)));

    APP_ERROR_CHECK(sd_ble_gap_appearance_set(ble_es_common::APPEARANCE));

    ble_gap_conn_params_t params = {
        .min_conn_interval = BLE_PERIPHERAL_MIN_CONN_INTERVAL,
        .max_conn_interval = BLE_PERIPHERAL_MAX_CONN_INTERVAL,
        .slave_latency     = BLE_PERIPHERAL_SLAVE_LATENCY,
        .conn_sup_timeout  = BLE_PERIPHERAL_CONN_SUP_TIMEOUT,
    };

    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&params));
}

static void advertising_init() {
    ble_advertising_init_t adv_init = {
        .config = {
            .ble_adv_fast_enabled  = true,
            .ble_adv_fast_interval = BLE_PERIPHERAL_ADV_INTERVAL,
            .ble_adv_fast_timeout  = BLE_PERIPHERAL_ADV_DURATION,
        },
        // TODO(CMK) 06/19/20: advertising event handler
        .evt_handler = nullptr,
        .error_handler = nullptr,
    };

    APP_ERROR_CHECK(ble_advertising_init(g_advertising, &adv_init));

    ble_advertising_conn_cfg_tag_set(g_advertising, BLE_COMMON_CONN_CFG_TAG);
}

static void conn_params_init() {
    ble_conn_params_init_t cp_init = {
        .p_conn_params                  = nullptr,
        .first_conn_params_update_delay = BLE_PERIPHERAL_FIRST_CONN_PARAMS_UPDATE_DELAY,
        .next_conn_params_update_delay  = BLE_PERIPHERAL_NEXT_CONN_PARAMS_UPDATE_DELAY,
        .max_conn_params_update_count   = BLE_PERIPHERAL_MAX_CONN_PARAMS_UPDATE_COUNT,
        .disconnect_on_fail             = false,
        /* TODO(CMK) 06/19/20: event and error handlers */
        .evt_handler                    = nullptr,
        .error_handler                  = nullptr,
    };

    APP_ERROR_CHECK(ble_conn_params_init(&cp_init));
}

static void db_discovery_init(ble_db_discovery_evt_handler_t handler) {
    ble_db_discovery_init_t db_init {
        .evt_handler = handler,
        .p_gatt_queue = g_gatt_queue,
    };

    APP_ERROR_CHECK(ble_db_discovery_init(&db_init));
}

}  // namespace ble_peripheral
