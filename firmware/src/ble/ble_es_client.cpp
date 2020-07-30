/*
 * ble_es_client.cpp - Client for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_es_client.hpp"

#include <ble_srv_common.h>

void BLEESClient::init(nrf_ble_gq_t *gatt_queue) {
    /* Default init member variables */
    _es_hall_handle = BLE_GATT_HANDLE_INVALID;
    _es_hall_cccd_handle = BLE_GATT_HANDLE_INVALID;
    _conn_handle = BLE_CONN_HANDLE_INVALID;
    _callback = {};
    _gatt_queue = gatt_queue;

    BLEESCommon::init();

    /* Add custom electric skateboard service */
    ble_uuid_t uuid {
        .uuid = BLEESCommon::UUID_SERVICE,
        .type = BLEESCommon::uuid_type(),
    };

    APP_ERROR_CHECK(
        ble_db_discovery_evt_register(&uuid));
}

void BLEESClient::event_handler(ble_evt_t const *p_ble_evt, void *p_context) {
    auto *_this = reinterpret_cast<BLEESClient *>(p_context);

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED: {
            _this->_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        } break;

        case BLE_GAP_EVT_DISCONNECTED: {
            _this->_conn_handle = BLE_CONN_HANDLE_INVALID;
        };
    }
}

void BLEESClient::on_db_discovery_evt(const ble_db_discovery_evt_t *p_evt) {
    switch (p_evt->evt_type) {
        case BLE_DB_DISCOVERY_COMPLETE: {
            const auto &discovered_db = p_evt->params.discovered_db;
            if (discovered_db.srv_uuid.uuid == BLEESCommon::UUID_SERVICE &&
                discovered_db.srv_uuid.type == BLEESCommon::uuid_type()) {
                const auto &characteristics = discovered_db.charateristics;
                for (unsigned i = 0; i < discovered_db.char_count; ++i) {
                    if (characteristics[i].characteristic.uuid.uuid == BLEESCommon::UUID_SENSOR_CHAR) {
                        _es_hall_handle = characteristics[i].characteristic.handle_value;
                        _es_hall_cccd_handle = characteristics[i].cccd_handle;
                    }
                }

                subscribe_to_notifications();
            }
        } break;
    }
}

void BLEESClient::subscribe_to_notifications() {
    ASSERT(_es_hall_cccd_handle != BLE_GATT_HANDLE_INVALID &&
           _gatt_queue != nullptr &&
           _conn_handle != BLE_CONN_HANDLE_INVALID);

    std::uint8_t cccd[BLE_CCCD_VALUE_LEN] = {
        [0] = LSB_16(BLE_GATT_HVX_NOTIFICATION),
        [1] = MSB_16(BLE_GATT_HVX_NOTIFICATION),
    };

    nrf_ble_gq_req_t cccd_req = {
        .type = NRF_BLE_GQ_REQ_GATTC_WRITE,
        .p_mem_obj = nullptr,
        .error_handler {
            .cb = nullptr,  // TODO(CMK) 07/27/20: error handler
            .p_ctx = nullptr
        },
        .params {
            .gattc_write = {
                .write_op = BLE_GATT_OP_WRITE_REQ,
                .flags = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE,
                .handle = _es_hall_cccd_handle,
                .offset = 0,
                .len = BLE_CCCD_VALUE_LEN,
                .p_value = cccd,
            }
        }
    };

    nrf_ble_gq_item_add(_gatt_queue, &cccd_req, _conn_handle);
}
