/*
 * util.hpp - miscellaneous utilities.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */
 
#include "util.hpp"

#include <bsp.h>
#include <bsp_btn_ble.h>
#include <nrf.h>
#include <nrf_drv_clock.h>
#include <nrf_soc.h>
#include <sdk_errors.h>

void Util::clock_init()
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}

void Util::enable_deep_sleep()
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

void Util::enter_sleep_mode()
{
    ret_code_t err_code;

    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}
