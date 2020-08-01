/*
 * app_config.h - application configuration.
 *
 * Holds configurations for the electric skateboard remote.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "boards_inc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Main App Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Priority for BLE events to be dispatched to custom electric skateboard service. */
#define BLE_ES_OBSERVER_PRIO 2

////////////////////////////////////////////////////////////////////////////////////////////////////
// BLE Common Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Priority for BLE events to be dispatched to common BLE event handler. */
#define BLE_COMMON_OBSERVER_PRIO 3

/**< A tag identifying the SoftDevice BLE configuration (used when creating connections). */
#define BLE_COMMON_CONN_CFG_TAG 1

/**< Perform bonding. */
#define BLE_COMMON_SEC_PARAM_BOND true

/**< Man In The Middle protection. */
#define BLE_COMMON_SEC_PARAM_MITM false

/**< LE Secure Connections. */
#define BLE_COMMON_SEC_PARAM_LESC false

/**< Keypress notifications. */
#define BLE_COMMON_SEC_PARAM_KEYPRESS false

/**< No I/O capabilities. */
#define BLE_COMMON_SEC_PARAM_IO_CAPABILITIES BLE_GAP_IO_CAPS_NONE

/**< Out Of Band data. */
#define BLE_COMMON_SEC_PARAM_OOB false

/**< Minimum encryption key size. */
#define BLE_COMMON_SEC_PARAM_MIN_KEY_SIZE 7

/**< Maximum encryption key size. */
#define BLE_COMMON_SEC_PARAM_MAX_KEY_SIZE 16

/**< The FDS file ID for the paired device address file. */
#define BLE_COMMON_FDS_ADDR_FILE_ID 0x0001

/**< The FDS record key for the paired device address. */
#define BLE_COMMON_FDS_ADDR_RECORD_KEY 0x0001

////////////////////////////////////////////////////////////////////////////////////////////////////
// BLE Peripheral Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Name of device. Will be included in the advertising data. */
#define BLE_PERIPHERAL_DEVICE_NAME "Esk8 Remote"

/**< Manufacturer. Will be passed to Device Information Service. */
#define BLE_PERIPHERAL_MANUFACTURER_NAME "N/A"

/**< The advertising interval. */
#define BLE_PERIPHERAL_ADV_INTERVAL ((uint32_t) MSEC_TO_UNITS(187.5, UNIT_0_625_MS))

/**< The advertising duration. */
#define BLE_PERIPHERAL_ADV_DURATION ((uint32_t) MSEC_TO_UNITS(180000, UNIT_10_MS))

/**< Minimum acceptable connection interval (0.4 seconds). */
#define BLE_PERIPHERAL_MIN_CONN_INTERVAL ((uint32_t) MSEC_TO_UNITS(400, UNIT_1_25_MS))

/**< Maximum acceptable connection interval (0.65 second). */
#define BLE_PERIPHERAL_MAX_CONN_INTERVAL ((uint32_t) MSEC_TO_UNITS(650, UNIT_1_25_MS))

/**< Slave latency. */
#define BLE_PERIPHERAL_SLAVE_LATENCY 0

/**< Connection supervisory time-out. */
#define BLE_PERIPHERAL_CONN_SUP_TIMEOUT ((uint32_t) MSEC_TO_UNITS(4000, UNIT_10_MS))

/**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define BLE_PERIPHERAL_FIRST_CONN_PARAMS_UPDATE_DELAY 5000

/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define BLE_PERIPHERAL_NEXT_CONN_PARAMS_UPDATE_DELAY 30000

/**< Number of attempts before giving up the connection parameter negotiation. */
#define BLE_PERIPHERAL_MAX_CONN_PARAMS_UPDATE_COUNT 3

////////////////////////////////////////////////////////////////////////////////////////////////////
// BLE Central Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Scan interval. */
#define BLE_CENTRAL_SCAN_INTERVAL ((uint32_t) MSEC_TO_UNITS(100, UNIT_0_625_MS))

/**< Scan window. */
#define BLE_CENTRAL_SCAN_WINDOW ((uint32_t) MSEC_TO_UNITS(50, UNIT_0_625_MS))

/**< Minimum connection interval. */
#define BLE_CENTRAL_MIN_CONN_INTERVAL ((uint32_t) MSEC_TO_UNITS(7.5, UNIT_1_25_MS))

/**< Maximum connection interval */
#define BLE_CENTRAL_MAX_CONN_INTERVAL ((uint32_t) MSEC_TO_UNITS(30, UNIT_1_25_MS))

/**< Slave latency. */
#define BLE_CENTRAL_SLAVE_LATENCY 0

/**< Supervision timeout. */
#define BLE_CENTRAL_SUPERVISION_TIMEOUT ((uint32_t) MSEC_TO_UNITS(4000, UNIT_10_MS))

////////////////////////////////////////////////////////////////////////////////////////////////////
// Logger Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< If the logger should use the flash backend in addition to UART */
#define LOGGER_USE_FLASH_BACKEND false

////////////////////////////////////////////////////////////////////////////////////////////////////
// SDK Config Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Which pin the UART logger should transmit on - uses board-specific definition */
#ifndef NRF_LOG_BACKEND_UART_TX_PIN
#define NRF_LOG_BACKEND_UART_TX_PIN TX_PIN_NUMBER
#endif

#endif // APP_CONFIG_H
