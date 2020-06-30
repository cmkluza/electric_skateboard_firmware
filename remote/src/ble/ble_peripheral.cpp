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
static ret_code_t advertising_init();

/** Function for initializing services that will be used by the application. */
static ret_code_t services_init();

/** Function for initializing the Connection Parameters module. */
static ret_code_t conn_params_init();

/** Clear bond information from persistent storage. */
static void delete_bonds();

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

ret_code_t init(const ble_common::Config &config)
{
    /* Preconditions */
    ASSERT(config.type == ble_common::Config::ConfigType::PERIPHERAL);
    
    g_gatt = config.gatt;
    g_advertising = config.peripheral_config.advertising;
    
    ASSERT(g_gatt != nullptr &&
           g_advertising != nullptr);
    
    /* Ensure common BLE init takes place first */
    bool common_is_initialized = *static_cast<bool *>(config.ble_observer->p_context);
    
    if (!common_is_initialized) {
        ble_common::init(config);
    }
    
    /* Initialize BLE peripheral-specific modules */ 
    ret_code_t err_code;
    
    gap_init();
    
    if (NRF_SUCCESS != (err_code = advertising_init())) {
        return err_code;
    }
    
    if (NRF_SUCCESS != (err_code = services_init())) {
        return err_code;
    }
    
    if (NRF_SUCCESS != (err_code = conn_params_init())) {
        return err_code;
    }
    
    return NRF_SUCCESS;
}

void advertising_start(bool erase_bonds)
{
    if (erase_bonds) {
        delete_bonds();
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

static ret_code_t advertising_init()
{
    ret_code_t             err_code;
    
    ble_advertising_init_t init = {};

//        ble_advdata_t           advdata;       /**< Advertising data: name, appearance, discovery flags, and more. */
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

    err_code = ble_advertising_init(g_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(g_advertising, BLE_COMMON_CONN_CFG_TAG);
}

static ret_code_t services_init()
{
    ret_code_t         err_code;
    ble_dis_init_t     dis_init;

    // Initialize Device Information Service.
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)BLE_PERIPHERAL_MANUFACTURER_NAME);

    dis_init.dis_char_rd_sec = SEC_OPEN;

    err_code = ble_dis_init(&dis_init);
    APP_ERROR_CHECK(err_code);
}

static ret_code_t conn_params_init()
{
    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = nullptr;
    cp_init.first_conn_params_update_delay = BLE_PERIPHERAL_FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = BLE_PERIPHERAL_NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = BLE_PERIPHERAL_MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.disconnect_on_fail             = false;
    /* TODO CMK 06/19/20: event and error handlers */
    cp_init.evt_handler                    = nullptr;
    cp_init.error_handler                  = nullptr;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}



} // namespace ble_peripheral
