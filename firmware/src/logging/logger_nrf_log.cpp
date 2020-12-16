/*
 * logger_nrf_log.cpp - logging implementation that passes through to nRF's log library and
 *                      FreeRTOS.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "logger.hpp"  // NOLINT

#include <sdk_config.h>

#include <FreeRTOS.h>
#include <task.h>

#include <app_error.h>
#include <nrf_log_ctrl.h>
#include <nrf_log_default_backends.h>

namespace logger {
#if NRF_LOG_ENABLED

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Thread for handling the logger.
 *
 * This thread is responsible for processing log entries if logs are deferred.
 * Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in] arg context passed to the thread (nullptr).
 */
static void logger_thread(void *arg);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< FreeRTOS handle for the logger thread. */
static TaskHandle_t logger_thandle;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void init() {
    // TODO(CMK) 06/19/20: timestamp function
    APP_ERROR_CHECK(NRF_LOG_INIT(nullptr));

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // TODO(CMK) 06/18/20: logger flash backend

    if (pdPASS != xTaskCreate(logger_thread, "Logger", 256, nullptr, 1, &logger_thandle)) {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

void idle() {
     vTaskResume(logger_thandle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void logger_thread(void *arg) {
    UNUSED_PARAMETER(arg);

    while (true) {
        NRF_LOG_FLUSH();
        vTaskSuspend(nullptr);
    }
}

#else  // !NRF_LOG_ENABLED

void init() {}
void idle() {}

#endif  // NRF_LOG_ENABLED

}  // namespace logger
