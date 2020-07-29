/*
 * remote.cpp
 *
 * Main program for an electric skateboard remote controller.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_central.hpp"
#include "ble_remote.hpp"
#include "es_fds.hpp"
#include "logger.hpp"
#include "util.hpp"

#include <app_error.h>
#include <fds.h>
#include <nrf_log.h>
#include <nrf_sdh_freertos.h>
#include <sdk_errors.h>

#include <FreeRTOS.h>
#include <task.h>

int main()
{
    /* Early init */
    logger::init();

    /* Hardware and BSP initialization */
    util::clock_init();

    /* Library and module initialization */
    // TODO CMK 07/03/20: FDS
    //es_fds::init();
    
    /* BLE initialization */
    ble_remote::init();
    
    /* Setup the SDH thread to start scanning */
    nrf_sdh_freertos_init([](void *ignored) {
        ble_central::begin_scanning();
    }, nullptr);

    /* FreeRTOS initialization */
    NRF_LOG_INFO("FreeRTOS Starting");
    vTaskStartScheduler();

    /* Should never reach here */
    APP_ERROR_HANDLER(-1);
}

/**
 * FreeRTOS idle hook.
 *
 * Called repeatedly by the idle task, should perform very low priority
 * tasks.
 */
extern "C"
void vApplicationIdleHook(void)
{
    logger::idle();
    // TODO CMK 06/19/20: enter power saving here?
}
