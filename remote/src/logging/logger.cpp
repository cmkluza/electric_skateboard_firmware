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

#if NRF_LOG_ENABLED

static TaskHandle_t m_logger_thread;

/**
 * Thread for handling the logger.
 *
 * This thread is responsible for processing log entries if logs are deferred.
 * Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in] arg Pointer used for passing some arbitrary information (context) from the
 *                osThreadCreate() call to the thread.
 */
static void logger_thread(void * arg)
{
    UNUSED_PARAMETER(arg);

    while (1)
    {
        NRF_LOG_FLUSH();

        vTaskSuspend(NULL); // Suspend myself
    }
}

void Logger::init()
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }

    // TODO CMK: logger flash backend
}

void Logger::idle()
{
     vTaskResume(m_logger_thread);
}
 
#else // !NRF_LOG_ENABLED

void Logger::init() {}
void Logger::idle() {}

#endif // NRF_LOG_ENABLED