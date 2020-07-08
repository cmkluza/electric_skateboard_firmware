/*
 * main.cpp
 *
 * Main program for an electric skateboard remote controller.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_central.hpp"
#include "ble_remote.hpp"
#include "esk8_fds.hpp"
#include "logger.hpp"
#include "util.hpp"

#include <app_error.h>
#include <fds.h>
#include <nrf_log.h>
#include <nrf_sdh_freertos.h>
#include <sdk_errors.h>

#include <FreeRTOS.h>
#include <task.h>

/** BLE Notes
 *
 * Remote-Receiver Relations:
 *  - The remote shall be a BLE central. The receiver shall be a BLE peripheral.
 *  - Upon startup, the receiver:
 *      - If bonded: continuously attempts to connect (bond can be cleared w/ button hold or press)
 *      - If not bonded: advertises
 *    The remote:
 *      - If bonded: continuously attempts to connect
 *      - If not bonded: reads advertisements, looking for receivers
 *  - Upon connection, the remote starts a server to serve Hall effect sensor data to the receiver.
 *    The receiver starts a client that subscribes to the Hall effect sensor data and can update the 
 *    app when new data is available.
 *
 * Remote-Phone Relations:
 *  - To be determined
 *  - Could have remote be a BLE peripheral that phone connects to
 *  - Features could include getting date/time from phone, getting GPS/directions, firmware udpates,
 *    etc.
 */

// TODO CMK 07/03/20: rename everything, remove "esk8"
// TODO CMK 07/03/20: implement a "delete_bonds" type function
// TODO CMK 07/03/20: make proper electric skateboard BLE service/client
// TODO CMK 07/03/20: sensor simulator
// TODO CMK 06/24/20: power management
// TODO CMK 07/03/20: peer manager (?)
// TODO CMK 07/03/20: DB discovery (?)
// TODO CMK 07/03/20: GAP params (for advertising, if doing dual-role)
// TODO CMK 07/03/20: Advertising (if doing dual-role)
// TODO CMK 07/03/20: Connection parameters (if doing dual-role)

int main()
{
    /* Early init */
    logger::init();

    /* Hardware and BSP initialization */
    util::clock_init();

    /* Library and module initialization */
    // TODO CMK 07/03/20: FDS
    //esk8_fds::init();
    
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
