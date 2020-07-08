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
 
////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "app_config.h"
#include "ble_peripheral.hpp"
#include "util.hpp"

#include <app_error.h>
#include <ble_advertising.h>
#include <ble_conn_params.h>
#include <ble_dis.h>
// TODO CMK 06/21/20: implement battery service client
#include <ble_bas_c.h>
// TODO CMK 06/21/20: implement current time client for phone connection?
//#include <ble_cts_c.h>
#include <bsp.h>
#include <nrf_ble_gatt.h>
#include <nrf_log.h>
#include <nrf_sdh.h>
#include <nrf_sdh_ble.h>

namespace ble_peripheral {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Data
////////////////////////////////////////////////////////////////////////////////////////////////////

static uint16_t g_conn_handle { BLE_CONN_HANDLE_INVALID };
static ble_uuid_t g_adv_uuids[] =
{
    /* TODO CMK 06/21/20: Fill in type dynamically during init */
    //{BLEEsk8Server::UUID_SERVICE, BLE_UUID_TYPE_UNKNOWN},
    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
};

static nrf_ble_gatt_t *g_gatt; /**< nRF BLE GATT instance. */
static ble_advertising_t *g_advertising; /**< nRF BLE advertising instance. */

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Configures GAP parameters.
 *
 * Sets up the security mode, appearance, and connection parameters for this device.
 */
static void gap_init();

/** Function for initializing the Advertising functionality. */
static void advertising_init();

/** Function for initializing services that will be used by the application. */
static void services_init();

/** Function for initializing the Connection Parameters module. */
static void conn_params_init();

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init(const ble_common::Config &config)
{
    /* Preconditions */
    ASSERT(config.type == ble_common::Config::ConfigType::PERIPHERAL);
    
    g_gatt = config.gatt;
    g_advertising = config.peripheral_config.advertising;
    
    ASSERT(g_gatt != nullptr &&
           g_advertising != nullptr);
    
    ble_common::init(config);

    /* Initialize BLE peripheral-specific modules */ 
    gap_init();
    advertising_init();
    services_init();
    conn_params_init();
}

void advertising_start(bool erase_bonds)
{
    if (erase_bonds) {
        // TODO CMK 06/24/20: start advertising in event handler upon PM_EVT_PEERS_DELETE_SUCCEEDED 
        // Advertising is started by PM_EVT_PEERS_DELETE_SUCCEEDED event.
    } else {
        APP_ERROR_CHECK(ble_advertising_start(g_advertising, BLE_ADV_MODE_FAST));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void gap_init()
{
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    APP_ERROR_CHECK(
        sd_ble_gap_device_name_set(&sec_mode,
                                   reinterpret_cast<const uint8_t *>(BLE_PERIPHERAL_DEVICE_NAME),
                                   strlen(BLE_PERIPHERAL_DEVICE_NAME)));
    
    /* TODO CMK 06/20/20: is there a better appearance type/does it matter */
    APP_ERROR_CHECK(sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_CYCLING));

    ble_gap_conn_params_t params = {
        .min_conn_interval = BLE_PERIPHERAL_MIN_CONN_INTERVAL,
        .max_conn_interval = BLE_PERIPHERAL_MAX_CONN_INTERVAL,
        .slave_latency     = BLE_PERIPHERAL_SLAVE_LATENCY,
        .conn_sup_timeout  = BLE_PERIPHERAL_CONN_SUP_TIMEOUT,
    };

    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&params));
}

// TODO CMK 07/03/20: finish advertising init
static void advertising_init()
{
    ble_advertising_init_t init = {};

//    ble_advdata_t           advdata;       /**< Advertising data: name, appearance, discovery flags, and more. */
//    ble_advdata_t           srdata;        /**< Scan response data: Supplement to advertising data. */
//    ble_adv_modes_config_t  config;        /**< Select which advertising modes and intervals will be utilized.*/
//    ble_adv_evt_handler_t   evt_handler;   /**< Event handler that will be called upon advertising events. */
//    ble_adv_error_handler_t error_handler; /**< Error handler that will propogate internal errors to the main applications. */

    /* Advertising data (name, appearance, discovery flags, etc) */
    init.advdata.name_type = BLE_ADVDATA_FULL_NAME; /**< Advertise full device name */
    init.advdata.include_appearance = true;
    init.advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE; /**< Advertise indefinitely */
    

    // TODO CMK 06/19/20: advertising event handler
    init.evt_handler = nullptr;

    APP_ERROR_CHECK(ble_advertising_init(g_advertising, &init));

    ble_advertising_conn_cfg_tag_set(g_advertising, BLE_COMMON_CONN_CFG_TAG);
}

// TODO CMK 07/03/20: what is DIS, do I want it
// TODO CMK 07/03/20: add electric skateboard client
static void services_init()
{
    (void)10;
    return;
}

static void conn_params_init()
{
    ble_conn_params_init_t init = {};

    init.p_conn_params                  = nullptr;
    init.first_conn_params_update_delay = BLE_PERIPHERAL_FIRST_CONN_PARAMS_UPDATE_DELAY;
    init.next_conn_params_update_delay  = BLE_PERIPHERAL_NEXT_CONN_PARAMS_UPDATE_DELAY;
    init.max_conn_params_update_count   = BLE_PERIPHERAL_MAX_CONN_PARAMS_UPDATE_COUNT;
    init.disconnect_on_fail             = false;
    /* TODO CMK 06/19/20: event and error handlers */
    init.evt_handler                    = nullptr;
    init.error_handler                  = nullptr;

    APP_ERROR_CHECK(ble_conn_params_init(&init));
}



} // namespace ble_peripheral
