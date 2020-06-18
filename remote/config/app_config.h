/*
 * app_config.h - application configuration.
 *
 * Performs initialization and maintenance of various BLE functionalities.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "boards_inc.h"

/** If the logger should use the flash backend in addition to UART */
#ifndef USE_FLASH_LOGGER
#define USE_FLASH_LOGGER false
#endif

/** Which pin the UART logger should transmit on - uses board-specific definition */
#ifndef NRF_LOG_BACKEND_UART_TX_PIN 
#define NRF_LOG_BACKEND_UART_TX_PIN TX_PIN_NUMBER
#endif

#endif // APP_CONFIG_H
