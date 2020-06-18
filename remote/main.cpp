/*
 * main.cpp
 *
 * Main program for an electric skateboard remote controller.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_peripheral.hpp"
#include "logger.hpp"
#include "util.hpp"

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_bas.h"
#include "ble_hrs.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "sensorsim.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_freertos.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "bsp_btn_ble.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "fds.h"
#include "ble_conn_state.h"
#include "nrf_drv_clock.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"

#include <nrf_log.h>

/* TODO CMK:
 * - APP_ERROR_HANDLER
 */
 
#define DEAD_BEEF                           0xDEADBEEF                              /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

static bool     m_rr_interval_enabled = true;                       /**< Flag for enabling and disabling the registration of new RR interval measurements (the purpose of disabling this is just to test sending HRM without RR interval data. */

static sensorsim_cfg_t   m_battery_sim_cfg;                         /**< Battery Level sensor simulator configuration. */
static sensorsim_state_t m_battery_sim_state;                       /**< Battery Level sensor simulator state. */
static sensorsim_cfg_t   m_heart_rate_sim_cfg;                      /**< Heart Rate sensor simulator configuration. */
static sensorsim_state_t m_heart_rate_sim_state;                    /**< Heart Rate sensor simulator state. */
static sensorsim_cfg_t   m_rr_interval_sim_cfg;                     /**< RR Interval sensor simulator configuration. */
static sensorsim_state_t m_rr_interval_sim_state;                   /**< RR Interval sensor simulator state. */

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for initializing the sensor simulators. */
static void sensor_simulator_init(void)
{
    m_battery_sim_cfg.min          = BlePeripheral::MIN_BATTERY_LEVEL;
    m_battery_sim_cfg.max          = BlePeripheral::MAX_BATTERY_LEVEL;
    m_battery_sim_cfg.incr         = BlePeripheral::BATTERY_LEVEL_INCREMENT;
    m_battery_sim_cfg.start_at_max = true;

    sensorsim_init(&m_battery_sim_state, &m_battery_sim_cfg);

    m_heart_rate_sim_cfg.min          = BlePeripheral::MIN_HEART_RATE;
    m_heart_rate_sim_cfg.max          = BlePeripheral::MAX_HEART_RATE;
    m_heart_rate_sim_cfg.incr         = BlePeripheral::HEART_RATE_INCREMENT;
    m_heart_rate_sim_cfg.start_at_max = false;

    sensorsim_init(&m_heart_rate_sim_state, &m_heart_rate_sim_cfg);

    m_rr_interval_sim_cfg.min          = BlePeripheral::MIN_RR_INTERVAL;
    m_rr_interval_sim_cfg.max          = BlePeripheral::MAX_RR_INTERVAL;
    m_rr_interval_sim_cfg.incr         = BlePeripheral::RR_INTERVAL_INCREMENT;
    m_rr_interval_sim_cfg.start_at_max = false;

    sensorsim_init(&m_rr_interval_sim_state, &m_rr_interval_sim_cfg);
}

/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(bool * p_erase_bonds)
{
    ret_code_t err_code;
    bsp_event_t startup_event;

    /*
     * inits LEDs and buttons
     */
    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, BlePeripheral::bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    /*
     * Checks if any buttons are held down (if clear bond button is held, sets startup event accordingly)
     * Inits button events for advertising
     */
    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

    /*
     * Set this so that in main we can tell if we want to clear bonding data
     */
    *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}

/**@brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
extern "C"
void vApplicationIdleHook( void )
{
    Logger::idle();
}

int main(void)
{
    bool erase_bonds;

    Logger::init();
    Util::clock_init();

    // Do not start any interrupt that uses system functions before system initialisation.
    // The best solution is to start the OS before any other initalisation.

    Util::enable_deep_sleep();

    // TODO CMK: init/start any necessary timers for periodic services

    buttons_leds_init(&erase_bonds);
    sensor_simulator_init();
    
    BlePeripheral::init();

    // TODO CMK: init other FreeRTOS threads

    // Create a FreeRTOS task for the BLE stack.
    // The task will run advertising_start() before entering its loop.
    nrf_sdh_freertos_init(&BlePeripheral::advertising_start, &erase_bonds);

    NRF_LOG_INFO("HRS FreeRTOS example started.");
    // Start FreeRTOS scheduler.
    vTaskStartScheduler();

    for (;;)
    {
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}

