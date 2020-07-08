/*
 * ble_remote.cpp - Electric skateboard remote main BLE wrapper.
 *
 * The primary module for the electric skateboard remote's BLE functionality.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ble_remote.hpp"
#include "ble_central.hpp"
#include "ble_common.hpp"
#include "ble_esk8_server.hpp"
#include "esk8_fds.hpp"
#include "util.hpp"

#include "app_config.h"

#include <ble_db_discovery.h>
#include <ble_srv_common.h>
#include <nrf_ble_gatt.h>
#include <nrf_ble_gq.h>
#include <nrf_ble_scan.h>
#include <nrf_log.h>
#include <nrf_sdh_ble.h>

#include <string.h>

namespace ble_remote {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/** Register common BLE event handler. */
NRF_SDH_BLE_OBSERVER(g_ble_observer, BLE_COMMON_OBSERVER_PRIO, 
                     ble_common::event_handler, nullptr);

/**< nRF BLE GATT instance. */
NRF_BLE_GATT_DEF(g_gatt);

/**< nRF BLE scanner instance. */
NRF_BLE_SCAN_DEF(g_scan);

/**< nRF GATT queue instance. */
NRF_BLE_GQ_DEF(g_gatt_queue,
               NRF_SDH_BLE_CENTRAL_LINK_COUNT,
               NRF_BLE_GQ_QUEUE_SIZE);

/**< nRF BLE discovery instance. */
BLE_DB_DISCOVERY_DEF(g_discovery);

/**< Custom electric skateboard server instance. */
BLE_ESK8_SERVER_DEF(g_esk8);

/**< List of BLE UUIDs to be advertised. */
static ble_uuid_t g_adv_uuids[] =
{
    /* TODO CMK 06/21/20: Fill in type dynamically during init */
    {BLEEsk8Server::UUID_SERVICE, BLE_UUID_TYPE_UNKNOWN},
    {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
};
BLE_ADVERTISING_DEF(g_advertising);

/**< The BLE initialization context. */
// TODO CMK 07/05/20: does this need to be global?
static ble_common::Config g_config = {};

/**< The BLE address of the paired receiver. Set to 0 when no receiver is paired. */
static ble_gap_addr_t g_paired_addr;

/**< The server to send remote values to the receiver. */
static BLEEsk8Server g_server;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Checks FDS to see if a paired address is stored, and reads it into g_paired_addr if so.
 */
static void init_paired_addr();

/**
 * BLE scan event handler.
 */
static void scan_event_handler(scan_evt_t const *p_scan_evt);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init()
{   
    g_config.ble_observer               = &g_ble_observer;
    g_config.gatt                       = &g_gatt;
    g_config.central_config.scan        = &g_scan;
    g_config.central_config.scan_handler = &scan_event_handler;
    g_config.central_config.gatt_queue  = &g_gatt_queue;
    g_config.central_config.discovery   = &g_discovery;
    g_config.type                       = ble_common::Config::ConfigType::CENTRAL;
    
    ble_central::init(g_config);
    
    init_paired_addr();
    
    g_server.init();
    
    ble_uuid_t uuid = {
        .uuid = BLEEsk8Server::UUID_SERVICE,
        .type = g_server.uuid_type(),
    };
    // TODO CMK  07/01/20: choose what kind of scanning based on if g_paired_addr is found
    ble_central::set_uuid_appearance_scan_filter(uuid, BLEEsk8Server::APPEARANCE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void init_paired_addr()
{
#if 0 // TODO CMK 07/01/20: init paired addr w/ FDS
    fds_record_desc_t desc = {};
    
    if (esk8_fds::record_is_present(BLE_COMMON_FDS_ADDR_FILE_ID, 
                                    BLE_COMMON_FDS_ADDR_RECORD_KEY, &desc)) {
        if (esk8_fds::read_record(&desc, reinterpret_cast<uint8_t *>(&g_paired_addr), 
                sizeof(g_paired_addr)) == NRF_SUCCESS) {
            return;
        }
    }
    
    /* Either record was missing or corrupted, start anew. */
    memset(&g_paired_addr, 0, sizeof(g_paired_addr));
#endif
}

static void scan_event_handler(scan_evt_t const *p_scan_evt)
{
    switch (p_scan_evt->scan_evt_id) {
        case NRF_BLE_SCAN_EVT_FILTER_MATCH:
            {
                auto *adv_report = p_scan_evt->params.filter_match.p_adv_report;
                NRF_LOG_INFO("SCANNED: " MAC_FMT, 
                             MAC_ARGS(adv_report->peer_addr.addr));
                util::log_ble_data(&adv_report->data,
                                   "         ");
            }
            break;

        case NRF_BLE_SCAN_EVT_WHITELIST_REQUEST: /* Not using whitelists */
            break;

        case NRF_BLE_SCAN_EVT_WHITELIST_ADV_REPORT: /* Not using whitelists */
            break;

        case NRF_BLE_SCAN_EVT_NOT_FOUND:
            break;

        case NRF_BLE_SCAN_EVT_SCAN_TIMEOUT:
            NRF_LOG_INFO("Scan timeout");
            // TODO CMK 06/29/20: restart scanning upon timeout?
            break;

        case NRF_BLE_SCAN_EVT_SCAN_REQ_REPORT:
            break;

        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR: /* Error while trying to connect. */
            APP_ERROR_CHECK(p_scan_evt->params.connecting_err.err_code);
            break;

        case NRF_BLE_SCAN_EVT_CONNECTED:
            g_paired_addr = p_scan_evt->params.connected.p_connected->peer_addr;
            NRF_LOG_INFO("CONNECTED: Handle 0x%04X", p_scan_evt->params.connected.conn_handle);
            NRF_LOG_INFO("           " MAC_FMT, MAC_ARGS(g_paired_addr.addr));
            // TODO CMK 06/29/20: more connection handling?
            break;
    }
}

}; // namespace ble_remote
