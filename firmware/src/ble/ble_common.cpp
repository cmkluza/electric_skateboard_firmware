/*
 * ble_common.cpp - Common BLE functions.
 *
 * Contains common BLE functions, such as setting up the BLE stack.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_common.hpp"

#include <nrf_sdh.h>
#include <nrf_sdh_ble.h>
#include <peer_manager.h>
#include <peer_manager_handler.h>

#include <cstdint>

#include "config/app_config.h"

namespace ble_common {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/** Configures and initializes the SoftDevice. */
static void ble_stack_init();

/** Configures and initializes the peer manager (for handling security). */
static void peer_manager_init();

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Whether or not the common BLE init has already taken place. */
static bool g_initialized { false };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init(const Data &data) {
    if (g_initialized) {
        return;
    }

    ble_stack_init();

    // TODO(CMK) 06/19/20: GATT event handler (?)
    APP_ERROR_CHECK(nrf_ble_gatt_init(data.gatt, nullptr));

    g_initialized = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void ble_stack_init() {
    APP_ERROR_CHECK(nrf_sdh_enable_request());

    std::uint32_t ram_start = {};
    APP_ERROR_CHECK(nrf_sdh_ble_default_cfg_set(BLE_COMMON_CONN_CFG_TAG, &ram_start));

    APP_ERROR_CHECK(nrf_sdh_ble_enable(&ram_start));
}

// TODO(CMK) 07/31/20: implement
static void peer_manager_init() {
    ble_gap_sec_params_t sec_param;
    ret_code_t           err_code;

    APP_ERROR_CHECK(pm_init());

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond           = BLE_COMMON_SEC_PARAM_BOND;
    sec_param.mitm           = BLE_COMMON_SEC_PARAM_MITM;
    sec_param.lesc           = BLE_COMMON_SEC_PARAM_LESC;
    sec_param.keypress       = BLE_COMMON_SEC_PARAM_KEYPRESS;
    sec_param.io_caps        = BLE_COMMON_SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob            = BLE_COMMON_SEC_PARAM_OOB;
    sec_param.min_key_size   = BLE_COMMON_SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size   = BLE_COMMON_SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc  = 1;
    sec_param.kdist_own.id   = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    // TODO(CMK) 06/19/20: event handler
    APP_ERROR_CHECK(pm_register(nullptr));
}

}  // namespace ble_common
