/*
 * ble_es_client.cpp - Client for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_es_client.hpp"

#include <ble_srv_common.h>

#include "logger.hpp"
using logger::Level;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void BLEESClient::init(nrf_ble_gq_t *gatt_queue) {
    /* Default init member variables */
    _es_hall_handle = BLE_GATT_HANDLE_INVALID;
    _es_hall_cccd_handle = BLE_GATT_HANDLE_INVALID;
    _conn_handle = BLE_CONN_HANDLE_INVALID;
    _callback = {};
    _gatt_queue = gatt_queue;

    ble_es_common::init();

    /* Add custom electric skateboard service */
    ble_uuid_t uuid {
        .uuid = ble_es_common::UUID_SERVICE,
        .type = ble_es_common::uuid_type(),
    };

    APP_ERROR_CHECK(
        ble_db_discovery_evt_register(&uuid));
}

void BLEESClient::event_handler(ble_evt_t const *p_ble_evt, void *p_context) {
    auto *_this = reinterpret_cast<BLEESClient *>(p_context);

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED: {
            _this->_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            APP_ERROR_CHECK(
                nrf_ble_gq_conn_handle_register(_this->_gatt_queue, _this->_conn_handle));
        } break;

        case BLE_GAP_EVT_DISCONNECTED: {
            _this->_conn_handle = BLE_CONN_HANDLE_INVALID;
        } break;

        case BLE_GATTC_EVT_HVX: {
            auto &gattc_evt = p_ble_evt->evt.gattc_evt;
            auto &hvx_evt = gattc_evt.params.hvx;

            if (gattc_evt.gatt_status != BLE_GATT_STATUS_SUCCESS) {
                APP_ERROR_HANDLER(gattc_evt.gatt_status);
            }

            logger::log<Level::INFO>("Recieved notification handle %04X", hvx_evt.handle);

            if (hvx_evt.handle == _this->_es_hall_handle) {
                if (_this->_callback) {
                    _this->_callback(HallSensor::from_bytes(hvx_evt.data));
                }
            }

            auto ret = sd_ble_gattc_hv_confirm(gattc_evt.conn_handle, hvx_evt.handle);
            if (ret != NRF_SUCCESS) {
                logger::log<Level::INFO>("%s::sd_ble_gattc_hv_confirm: 0x%08X", ret);
            }
        } break;
    }
}

void BLEESClient::on_db_discovery_evt(const ble_db_discovery_evt_t *p_evt) {
    switch (p_evt->evt_type) {
        case BLE_DB_DISCOVERY_COMPLETE: {
            const auto &discovered_db = p_evt->params.discovered_db;

            if (discovered_db.srv_uuid.uuid == ble_es_common::UUID_SERVICE &&
                discovered_db.srv_uuid.type == ble_es_common::uuid_type())
            { // NOLINT
                logger::log<Level::DBG>("DB discovery complete");
                const auto &characteristics = discovered_db.charateristics;

                for (unsigned i = 0; i < discovered_db.char_count; ++i) {
                    const auto &uuid = characteristics[i].characteristic.uuid.uuid;

                    if (uuid == ble_es_common::UUID_SENSOR_CHAR) {
                        _es_hall_handle = characteristics[i].characteristic.handle_value;
                        _es_hall_cccd_handle = characteristics[i].cccd_handle;
                    }
                }

                subscribe_to_notifications();
            }
        } break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

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

    auto ret = nrf_ble_gq_item_add(_gatt_queue, &cccd_req, _conn_handle);
    if (ret != NRF_SUCCESS) {
        logger::log<Level::INFO>("%s::nrf_ble_gq_item_add: 0x%08X", __func__, ret);
    }

    logger::log<Level::DBG>("Subscribed to CCCD notifications");
}
