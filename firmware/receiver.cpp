/*
 * receiver.cpp
 *
 * Main program for an electric skateboard receiver.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include <app_error.h>
#include <fds.h>
#include <nrf_log.h>
#include <nrf_sdh_freertos.h>
#include <sdk_errors.h>

#include <FreeRTOS.h>
#include <task.h>

#include "ble_peripheral.hpp"
#include "ble_receiver.hpp"
#include "es_fds.hpp"
#include "hall_sensor.hpp"
#include "logger.hpp"
#include "util.hpp"

// TODO(CMK) 08/01/20: move to a separate module
static void handle_sensor_data(HallSensor::type sensor_data);

int main() {
    /* Early init */
    logger::init();

    /* Hardware and BSP initialization */
    util::clock_init();

    /* Library and module initialization */
    // TODO(CMK) 07/03/20: FDS
    // es_fds::init();

    /* BLE initialization */
    ble_receiver::init(handle_sensor_data);

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
void vApplicationIdleHook(void) {
    logger::idle();
    // TODO(CMK) 06/19/20: enter power saving here?
}

static void handle_sensor_data(HallSensor::type sensor_data) {
    NRF_LOG_INFO("Recieved sensor data: 0x%04X", sensor_data);
}
