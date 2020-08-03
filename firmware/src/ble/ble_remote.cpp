/*
 * ble_remote.cpp - Electric skateboard remote main BLE wrapper.
 *
 * The primary module for the electric skateboard remote's BLE functionality.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_remote.hpp"

#include <ble_srv_common.h>
#include <nrf_ble_gatt.h>
#include <nrf_ble_gq.h>
#include <nrf_ble_scan.h>
#include <nrf_log.h>
#include <nrf_sdh_ble.h>
#include <nrf_sdh_freertos.h>

#include <cstring>

#include "ble_central.hpp"
#include "ble_common.hpp"
#include "ble_es_server.hpp"
#include "ble_events.hpp"
#include "config/app_config.h"
#include "es_fds.hpp"
#include "util.hpp"

namespace ble_remote {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/** Checks FDS to see if a paired address is stored, and reads it into g_paired_addr if so. */
static void init_paired_addr();

/**
 * BLE scan event handler.
 *
 * @param[in] p_scan_evt the scan event.
 */
static void scan_event_handler(scan_evt_t const *p_scan_evt);

/**
 * BLE event handler.
 *
 * @param[in] p_ble_evt the BLE event.
 * @param[in] p_context context passed when this handler is registered (nullptr).
 */
static void ble_event_handler(ble_evt_t const *p_ble_evt, void *p_context);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/** Register common BLE event handler. */
NRF_SDH_BLE_OBSERVER(g_ble_observer, BLE_COMMON_OBSERVER_PRIO,
                     ble_event_handler, nullptr);

/**< nRF BLE GATT instance. */
NRF_BLE_GATT_DEF(g_gatt);

/**< nRF BLE scanner instance. */
NRF_BLE_SCAN_DEF(g_scan);

/**< nRF GATT queue instance. */
NRF_BLE_GQ_DEF(g_gatt_queue,
               NRF_SDH_BLE_CENTRAL_LINK_COUNT,
               NRF_BLE_GQ_QUEUE_SIZE);

/**< Custom electric skateboard server instance. */
BLE_ES_SERVER_DEF(g_es_server);

/**< The BLE address of the paired receiver. Set to 0 when no receiver is paired. */
static ble_gap_addr_t g_paired_addr;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init() {
    auto data = ble_common::Data {
        .gatt                   = &g_gatt,
        .gatt_queue             = &g_gatt_queue,
        .scan                   = &g_scan,
        .scan_handler           = &scan_event_handler,
        .advertising            = nullptr,
        .discovery              = nullptr,
        .db_discovery_handler   = nullptr,
    };

    ble_central::init(data);

    init_paired_addr();

    g_es_server.init();

    ble_uuid_t uuid = {
        .uuid = ble_es_common::UUID_SERVICE,
        .type = ble_es_common::uuid_type(),
    };

    // TODO(CMK) 07/01/20: choose what kind of scanning based on if g_paired_addr is found
    ble_central::set_uuid_appearance_scan_filter(uuid, ble_es_common::APPEARANCE);

    /* Setup the SDH thread to start scanning */
    nrf_sdh_freertos_init([](void *ignored) {
        ble_central::begin_scanning();
    }, nullptr);
}

void update_sensor_value(HallSensor::type value) {
    g_es_server.update_sensor_value(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void init_paired_addr() {
#if 0  // TODO(CMK) 07/01/20: init paired addr w/ FDS
    fds_record_desc_t desc = {};

    if (es_fds::record_is_present(BLE_COMMON_FDS_ADDR_FILE_ID,
                                    BLE_COMMON_FDS_ADDR_RECORD_KEY, &desc)) {
        if (es_fds::read_record(&desc, reinterpret_cast<std::uint8_t *>(&g_paired_addr),
                sizeof(g_paired_addr)) == NRF_SUCCESS) {
            return;
        }
    }

    /* Either record was missing or corrupted, start anew. */
    memset(&g_paired_addr, 0, sizeof(g_paired_addr));
#endif
}

static void ble_event_handler(ble_evt_t const *p_ble_evt, void *p_context) {
    ble_events::Event event {};

    switch (p_ble_evt->header.evt_id) {
        /** BLE GAP events */

        case BLE_GAP_EVT_CONNECTED:
        {
            const auto &gap_evt = p_ble_evt->evt.gap_evt;
            const auto &connected_evt = gap_evt.params.connected;

            NRF_LOG_INFO("Connected to " MAC_FMT, MAC_ARGS(connected_evt.peer_addr.addr));
            NRF_LOG_INFO("Connection handle 0x%X", gap_evt.conn_handle);

            g_paired_addr = connected_evt.peer_addr;

            event.event = ble_events::Events::CONNECTED;
            event.data.connected.address = connected_evt.peer_addr.addr;

            ble_events::trigger_event(&event);

            // TODO(CMK) 07/11/20: more connection handling?
        } break;

        case BLE_GAP_EVT_DISCONNECTED:
        {
            const auto &gap_evt = p_ble_evt->evt.gap_evt;
            const auto &disconnected_evt = gap_evt.params.disconnected;

            NRF_LOG_INFO("Disconnected from 0x%X (reason: 0x%X)",
                         gap_evt.conn_handle, disconnected_evt.reason);

            event.event = ble_events::Events::DISCONNECTED;
            event.data.disconnected.address = g_paired_addr.addr;
            event.data.disconnected.reason = disconnected_evt.reason;

            ble_events::trigger_event(&event);

            // TODO(CMK) 07/11/20: more disconnection handling? start scanning?
        } break;

        case BLE_GAP_EVT_TIMEOUT:
        {
            const auto &gap_evt = p_ble_evt->evt.gap_evt;
            const auto &timeout_evt = gap_evt.params.timeout;

            /* Handle connection timeout */
            if (timeout_evt.src == BLE_GAP_TIMEOUT_SRC_CONN) {
                NRF_LOG_INFO("Timed out connecting");
                // TODO(CMK) 07/11/20: retry connection?
            }
        } break;

        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
        {
            const auto &gap_evt = p_ble_evt->evt.gap_evt;
            const auto &conn_params = gap_evt.params.conn_param_update_request.conn_params;

            APP_ERROR_CHECK(sd_ble_gap_conn_param_update(gap_evt.conn_handle, &conn_params));
        } break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            const auto &gap_evt = p_ble_evt->evt.gap_evt;

            ble_gap_phys_t phys = { BLE_GAP_PHY_AUTO, BLE_GAP_PHY_AUTO };
            APP_ERROR_CHECK(sd_ble_gap_phy_update(gap_evt.conn_handle, &phys));
        } break;

        /** BLE GATT client events */

        case BLE_GATTC_EVT_TIMEOUT:
        {
            const auto &gattc_evt = p_ble_evt->evt.gattc_evt;

            /* Lost communication to peripheral */
            NRF_LOG_INFO("GATTC timeout");
            APP_ERROR_CHECK(sd_ble_gap_disconnect(gattc_evt.conn_handle,
                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
        } break;

        /** BLE GATT server events */

        case BLE_GATTS_EVT_TIMEOUT:
        {
            const auto &gatts_evt = p_ble_evt->evt.gatts_evt;

            /* Lost communication to peripheral */
            NRF_LOG_INFO("GATTS timeout");
            APP_ERROR_CHECK(sd_ble_gap_disconnect(gatts_evt.conn_handle,
                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION));
        } break;
    }
}

static void scan_event_handler(scan_evt_t const *p_scan_evt) {
    switch (p_scan_evt->scan_evt_id) {
        case NRF_BLE_SCAN_EVT_FILTER_MATCH:
            {
                auto *adv_report = p_scan_evt->params.filter_match.p_adv_report;
                NRF_LOG_INFO("SCANNED: " MAC_FMT,
                             MAC_ARGS(adv_report->peer_addr.addr));
                util::log_ble_data(&adv_report->data);
            }
            break;

        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR: /* Error while trying to connect. */
            APP_ERROR_CHECK(p_scan_evt->params.connecting_err.err_code);
            break;
    }
}

}  // namespace ble_remote
