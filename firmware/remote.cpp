/*
 * remote.cpp
 *
 * Main program for an electric skateboard remote controller.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */


#include <app_error.h>
#include <app_timer.h>
#include <fds.h>
#include <nrf_log.h>
#include <sdk_errors.h>

// TODO - no?
#include <nrf_stack_guard.h>

#include <sensorsim.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "ble_central.hpp"
#include "ble_events.hpp"
#include "ble_remote.hpp"
#include "es_fds.hpp"
#include "logger.hpp"
#include "util.hpp"

// TODO(CMK) 08/01/20: testing
static void on_cccd_write(ble_events::Event *event);
static TimerHandle_t hall_sensor_timer;
static HallSensor hallSensor {};
static void hall_sensor_timeout_handler(TimerHandle_t xTimer);

int main() {
    /* Early init */
    logger::init();

    /* Stack guard - TODO - no? */
    NRF_STACK_GUARD_INIT();

    /* Hardware and BSP initialization */
    util::clock_init();

    hallSensor.init();
    APP_ERROR_CHECK(app_timer_init());
    hall_sensor_timer = xTimerCreate("Hall sens",
                                     5000, /* interval, ms */
                                     pdTRUE, /* auto reload */
                                     nullptr, /* timer ID */
                                     hall_sensor_timeout_handler);

    /* Library and module initialization */
    // TODO(CMK) 07/03/20: FDS

    /* BLE initialization */
    ble_remote::init();
    ble_events::register_event(ble_events::Events::CCCD_WRITE, on_cccd_write);

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

static void on_cccd_write(ble_events::Event *event) {
    NRF_LOG_INFO("Connected callback, starting timer");

    if (event->data.cccd_write.notifications_enabled) {
        xTimerStart(hall_sensor_timer, 0);
    } else {
        xTimerStop(hall_sensor_timer, 0);
    }
}

static void hall_sensor_timeout_handler(TimerHandle_t xTimer) {
    auto val = hallSensor.read();
    ble_remote::update_sensor_value(val);
}
