/*
 * ble_es_server.cpp - Server for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_es_server.hpp"

#include <app_error.h>
#include <ble.h>
#include <ble_gatts.h>
#include <ble_srv_common.h>
#include <ble_types.h>
#include <nrf_log.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void BLEESServer::init() {
    /* Initialize the service UUID */
    ble_es_common::init();

    /* Add custom electric skateboard service */
    ble_uuid_t uuid {
        .uuid = ble_es_common::UUID_SERVICE,
        .type = ble_es_common::uuid_type(),
    };
    APP_ERROR_CHECK(
        sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &uuid, &_service_handle));

    /* Add sensor data characteristic */
    HallSensor::type sensor_initial_value = {};
    ble_add_char_params_t add_char_params = {};

    /* Sensor characteristic */
    add_char_params.uuid      = { ble_es_common::UUID_SENSOR_CHAR };
    add_char_params.uuid_type = { ble_es_common::uuid_type() };

    /* Fixed length, initialize to 0 */
    add_char_params.max_len      = { sizeof(HallSensor::type) };
    add_char_params.init_len     = { add_char_params.max_len };
    add_char_params.p_init_value = { reinterpret_cast<std::uint8_t *>(&sensor_initial_value) };
    add_char_params.is_var_len   = { false };

    /* Read-only w/ notifications */
    add_char_params.char_props.broadcast       = { false };
    add_char_params.char_props.read            = { true };
    add_char_params.char_props.write_wo_resp   = { false };
    add_char_params.char_props.write           = { false };
    add_char_params.char_props.notify          = { true };
    add_char_params.char_props.indicate        = { false };
    add_char_params.char_props.auth_signed_wr  = { false };
    add_char_params.char_ext_props.reliable_wr = { false };
    add_char_params.char_ext_props.wr_aux      = { false };

    /* No deferred operations */
    add_char_params.is_defered_read  = { false };
    add_char_params.is_defered_write = { false };

    /* TODO(CMK) 06/22/20: security */
    add_char_params.read_access       = { SEC_OPEN };
    add_char_params.write_access      = { SEC_NO_ACCESS };  // no writing
    add_char_params.cccd_write_access = { SEC_OPEN };

    /* TODO(CMK) 06/22/20: once sensor module is developed - easier to use
                           local mem? */
    add_char_params.is_value_user = { false };

    /* No characteristic user description descriptor */
    add_char_params.p_user_descr = { nullptr };

    /* No characteristic presentation format descriptor */
    add_char_params.p_presentation_format = { nullptr };

    APP_ERROR_CHECK(
        characteristic_add(_service_handle, &add_char_params, &_sensor_char_handles));
}

// TODO(CMK) 07/27/20: verify sd_ble_gatts_hvx both updates the value and issues the notification
void BLEESServer::update_sensor_value(HallSensor::type new_value) {
    if (_conn_handle == BLE_CONN_HANDLE_INVALID) {
        NRF_LOG_WARNING("Attempted update sensor char while disconnected");
        return;
    }

    NRF_LOG_INFO("%s: 0x%04X", __func__, new_value);

    std::uint16_t len = { sizeof(new_value) };

    ble_gatts_hvx_params_t params = {
        .handle = _sensor_char_handles.value_handle,
        .type   = BLE_GATT_HVX_NOTIFICATION,
        .offset = 0,
        .p_len  = &len,
        .p_data = reinterpret_cast<std::uint8_t *>(&new_value),
    };

    auto ret = sd_ble_gatts_hvx(_conn_handle, &params);
    if (ret != NRF_SUCCESS)
        NRF_LOG_INFO("sd_ble_gatts_hvx: %X", ret);
}

void BLEESServer::event_handler(ble_evt_t const *p_ble_evt, void *p_context) {
    auto *_this = reinterpret_cast<BLEESServer *>(p_context);

    switch (p_ble_evt->header.evt_id) {
        // TODO(CMK) 07/27/20: anticipate issues with multiple connections
        case BLE_GAP_EVT_CONNECTED: {
            _this->_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        } break;

        case BLE_GAP_EVT_DISCONNECTED: {
            _this->_conn_handle = BLE_CONN_HANDLE_INVALID;
        } break;
    }
}
