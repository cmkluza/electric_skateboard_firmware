/*
 * main.cpp
 *
 * Main program for an electric skateboard remote controller.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_remote.hpp"
#include "esk8_fds.hpp"
#include "logger.hpp"
#include "util.hpp"

#include <app_error.h>
#include <fds.h>
#include <sdk_errors.h>

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

// TODO CMK 06/24/20: power management

int main()
{
    /* Early init */
    logger::init();

    /* Hardware and BSP initialization */
    util::clock_init();

    /* Library and module initialization */
    esk8_fds::init();
    
    /* BLE initialization */
    ble_remote::init();
    
    /* FreeRTOS initialization */
    

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
