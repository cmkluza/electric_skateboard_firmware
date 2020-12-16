/*
 * custom_remote_board.h - board definitions for reviison 0.0.1 of the custom remote PCB.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "boards_gpio.h"

#define UNUSED_PIN              (-1)

/* Nordic BSP definitions */
#define LEDS_NUMBER             0

#define BUTTONS_NUMBER          4

#define UP_BUTTON               _nRF_GPIO_PIN_MAP(1, 15) /* A14 */
#define TRIGGER_BUTTON          _nRF_GPIO_PIN_MAP(1, 10) /* A20 */
#define DOWN_BUTTON             _nRF_GPIO_PIN_MAP(1, 13) /* A16 */
#define POWER_BUTTON            _nRF_GPIO_PIN_MAP(0, 2) /* A12 */
#define BUTTON_PULL             _nRF_GPIO_PIN_NOPULL /* Board has external pull resistors */

#define BUTTONS_ACTIVE_STATE    0

#define BUTTONS_LIST            { UP_BUTTON, TRIGGER_BUTTON, DOWN_BUTTON, POWER_BUTTON }

#define BSP_BUTTON_0            UP_BUTTON
#define BSP_BUTTON_1            TRIGGER_BUTTON
#define BSP_BUTTON_2            DOWN_BUTTON
#define BSP_BUTTON_3            POWER_BUTTON

#define RX_PIN_NUMBER           _nRF_GPIO_PIN_MAP(1, 6) /* R24 */
#define TX_PIN_NUMBER           _nRF_GPIO_PIN_MAP(1, 7) /* P23 */
#define CTS_PIN_NUMBER          UNUSED_PIN
#define RTS_PIN_NUMBER          UNUSED_PIN
#define HWFC                    false

#define BSP_QSPI_SCK_PIN        _nRF_GPIO_PIN_MAP(0, 19) /* AC15 */
#define BSP_QSPI_CSN_PIN        _nRF_GPIO_PIN_MAP(0, 17) /* AD12 */
#define BSP_QSPI_IO0_PIN        _nRF_GPIO_PIN_MAP(0, 20) /* AD16 */
#define BSP_QSPI_IO1_PIN        _nRF_GPIO_PIN_MAP(0, 21) /* AC17 */
#define BSP_QSPI_IO2_PIN        _nRF_GPIO_PIN_MAP(0, 22) /* AD18 */
#define BSP_QSPI_IO3_PIN        _nRF_GPIO_PIN_MAP(0, 23) /* AC19 */

/* Battery management I2C */
#define PMIC_SCL_PIN            _nRF_GPIO_PIN_MAP(0, 7) /* M2 */
#define PMIC_SDA_PIN            _nRF_GPIO_PIN_MAP(1, 8) /* P2 */

/* LCD */
#define LCD_SCK_PIN             _nRF_GPIO_PIN_MAP(1, 9) /* R1 */
#define LCD_MOSI_PIN            _nRF_GPIO_PIN_MAP(0, 8) /* N1 */
#define LCD_LED_PIN             _nRF_GPIO_PIN_MAP(0, 4) /* J1 */
#define LCD_DC_PIN              _nRF_GPIO_PIN_MAP(0, 12) /* U1 */
#define LCD_RST_N_PIN           _nRF_GPIO_PIN_MAP(0, 6) /* L1 */

/* Piezo buzzer */
#define BUZZER_PIN              _nRF_GPIO_PIN_MAP(0,29) /* A10 */

/* Battery divider analog in */
#define VBAT_PIN                _nRF_GPIO_PIN_MAP(0, 31) /* A8 */

/* Hall sensor analog in */
#define HALL_SENSE_PIN          _nRF_GPIO_PIN_MAP(0, 5) /* K2 */

#ifdef __cplusplus
}
#endif
