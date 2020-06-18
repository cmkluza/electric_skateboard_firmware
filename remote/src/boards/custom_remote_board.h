/*
 * custom_remote_board.h - board definitions for custom remote controller board.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#ifndef ESK8_CUSTOM_REMOTE_BOARD_H
#define ESK8_CUSTOM_REMOTE_BOARD_H

// TODO CMK: go through these all again once board is ready

#ifdef __cplusplus
extern "C" {
#endif

#include "boards_gpio.h"

#define UNUSED_PIN              (-1)

////////////////////////////////////////////////////////////////////////////////
// Common BSP Definitions
////////////////////////////////////////////////////////////////////////////////
#define LEDS_NUMBER             2

#define LED_1                   _nRF_GPIO_PIN_MAP(0, 3)
#define LED_2                   _nRF_GPIO_PIN_MAP(1, 10)
#define LED_START               LED_1
#define LED_STOP                LED_2

#define LEDS_ACTIVE_STATE       1

#define LEDS_LIST               { LED_1, LED_2 }

#define LEDS_INV_MASK           LEDS_MASK

#define BSP_LED_0               LED_1
#define BSP_LED_1               LED_2

#define BUTTONS_NUMBER          1

#define BUTTON_1                _nRF_GPIO_PIN_MAP(1, 2)
#define BUTTON_PULL             _nRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE    0

#define BUTTONS_LIST            { BUTTON_1 }

#define BSP_BUTTON_0            BUTTON_1

#define RX_PIN_NUMBER           _nRF_GPIO_PIN_MAP(0, 24)
#define TX_PIN_NUMBER           _nRF_GPIO_PIN_MAP(0, 25)
#define CTS_PIN_NUMBER          UNUSED_PIN
#define RTS_PIN_NUMBER          UNUSED_PIN
#define HWFC                    false

#define BSP_QSPI_SCK_PIN        _nRF_GPIO_PIN_MAP(0, 19) /* AC15 */
#define BSP_QSPI_CSN_PIN        _nRF_GPIO_PIN_MAP(0, 17) /* AD12 */
#define BSP_QSPI_IO0_PIN        _nRF_GPIO_PIN_MAP(0, 20) /* AD16 */
#define BSP_QSPI_IO1_PIN        _nRF_GPIO_PIN_MAP(0, 22) /* AC17 */
#define BSP_QSPI_IO2_PIN        _nRF_GPIO_PIN_MAP(0, 23) /* AD18 */
#define BSP_QSPI_IO3_PIN        _nRF_GPIO_PIN_MAP(0, 21) /* AC19 */

#define PMIC_SCL_PIN            _nRF_GPIO_PIN_MAP(0, 11) /* Pin T2 */
#define PMIC_SDA_PIN            _nRF_GPIO_PIN_MAP(0, 12) /* Pin U1 */

#ifdef __cplusplus
}
#endif

#endif // ESK8_CUSTOM_REMOTE_BOARD_H
