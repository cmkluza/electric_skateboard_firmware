/*
 * logger.cpp - utilities for logging.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "logger.hpp"

#include <sdk_config.h>

#include <FreeRTOS.h>
#include <task.h>

#include <app_error.h>
#include <nrf_log_ctrl.h>
#include <nrf_log_default_backends.h>

namespace logger {

#if NRF_LOG_ENABLED

static TaskHandle_t logger_thandle;

/**
 * Thread for handling the logger.
 *
 * This thread is responsible for processing log entries if logs are deferred.
 * Thread flushes all log entries and suspends. It is resumed by idle task hook.
 */
static void logger_thread(void *arg)
{
    UNUSED_PARAMETER(arg);

    while (true) {
        NRF_LOG_FLUSH();
        vTaskSuspend(NULL);
    }
}

void init()
{
    // TODO CMK 06/19/20: timestamp function
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    
    // TODO CMK 06/18/20: logger flash backend
    
    if (pdPASS != xTaskCreate(logger_thread, "Logger", 256, NULL, 1, &logger_thandle))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

void idle()
{
     vTaskResume(logger_thandle);
}

#else // !NRF_LOG_ENABLED

void init() {}
void idle() {}

#endif // NRF_LOG_ENABLED

} // namespace logger