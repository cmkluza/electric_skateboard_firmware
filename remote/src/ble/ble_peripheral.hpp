/*
 * ble_peripheral.hpp - BLE peripheral functionality.
 *
 * Performs initialization and maintenance of various BLE functionalities.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#ifndef ESK8_BLE_PERIPHERAL_H
#define ESK8_BLE_PERIPHERAL_H

#include <ble_gap.h>
#include <bsp.h>

namespace BlePeripheral {
    static inline constexpr const char *DEVICE_NAME { "CMK_Test" }; /**< Name of device. Will be included in the advertising data. */
    static inline constexpr const char *MANUFACTURER_NAME { "NA" }; /**< Manufacturer. Will be passed to Device Information Service. */
    
    /**< Application's BLE observer priority. You shouldn't need to modify this value.
     * @note must remain a macro to work with Nordic's macros. */
    #define APP_BLE_OBSERVER_PRIO 3
    
    static inline constexpr uint8_t APP_BLE_CONN_CFG_TAG { 1 }; /**< A tag identifying the SoftDevice BLE configuration. */
    static inline constexpr uint32_t APP_ADV_INTERVAL { 300 }; /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
    static inline constexpr uint32_t APP_ADV_DURATION { 18000 }; /**< The advertising duration (180 seconds) in units of 10 milliseconds. */
    
    static inline constexpr uint32_t BATTERY_LEVEL_MEAS_INTERVAL { 2000 }; /**< Battery level measurement interval (ms). */
    static inline constexpr uint32_t MIN_BATTERY_LEVEL { 81 }; /**< Minimum simulated battery level. */
    static inline constexpr uint32_t MAX_BATTERY_LEVEL { 100 }; /**< Maximum simulated battery level. */
    static inline constexpr uint32_t BATTERY_LEVEL_INCREMENT { 1 }; /**< Increment between each simulated battery level measurement. */

    static inline constexpr uint32_t HEART_RATE_MEAS_INTERVAL { 1000 }; /**< Heart rate measurement interval (ms). */
    static inline constexpr uint32_t MIN_HEART_RATE { 140 }; /**< Minimum heart rate as returned by the simulated measurement function. */
    static inline constexpr uint32_t MAX_HEART_RATE { 300 }; /**< Maximum heart rate as returned by the simulated measurement function. */
    static inline constexpr uint32_t HEART_RATE_INCREMENT { 10 }; /**< Value by which the heart rate is incremented/decremented for each call to the simulated measurement function. */

    static inline constexpr uint32_t RR_INTERVAL_INTERVAL { 300 }; /**< RR interval interval (ms). */
    static inline constexpr uint32_t MIN_RR_INTERVAL { 100 }; /**< Minimum RR interval as returned by the simulated measurement function. */
    static inline constexpr uint32_t MAX_RR_INTERVAL { 500 }; /**< Maximum RR interval as returned by the simulated measurement function. */
    static inline constexpr uint32_t RR_INTERVAL_INCREMENT { 1 }; /**< Value by which the RR interval is incremented/decremented for each call to the simulated measurement function. */

    static inline constexpr uint32_t SENSOR_CONTACT_DETECTED_INTERVAL { 5000 }; /**< Sensor Contact Detected toggle interval (ms). */

    static inline constexpr uint32_t MIN_CONN_INTERVAL { MSEC_TO_UNITS(400, UNIT_1_25_MS) }; /**< Minimum acceptable connection interval (0.4 seconds). */
    static inline constexpr uint32_t MAX_CONN_INTERVAL { MSEC_TO_UNITS(650, UNIT_1_25_MS) }; /**< Maximum acceptable connection interval (0.65 second). */
    static inline constexpr uint32_t SLAVE_LATENCY { 0 }; /**< Slave latency. */
    static inline constexpr uint32_t CONN_SUP_TIMEOUT { MSEC_TO_UNITS(4000, UNIT_10_MS) }; /**< Connection supervisory time-out (4 seconds). */

    static inline constexpr uint32_t FIRST_CONN_PARAMS_UPDATE_DELAY { 5000 }; /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
    static inline constexpr uint32_t NEXT_CONN_PARAMS_UPDATE_DELAY { 30000 }; /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
    static inline constexpr uint32_t MAX_CONN_PARAMS_UPDATE_COUNT { 3 }; /**< Number of attempts before giving up the connection parameter negotiation. */

    static inline constexpr uint32_t SEC_PARAM_BOND { 1 }; /**< Perform bonding. */
    static inline constexpr uint32_t SEC_PARAM_MITM { 0 }; /**< Man In The Middle protection not required. */
    static inline constexpr uint32_t SEC_PARAM_LESC { 0 }; /**< LE Secure Connections not enabled. */
    static inline constexpr uint32_t SEC_PARAM_KEYPRESS { 0 }; /**< Keypress notifications not enabled. */
    static inline constexpr uint32_t SEC_PARAM_IO_CAPABILITIES { BLE_GAP_IO_CAPS_NONE }; /**< No I/O capabilities. */
    static inline constexpr uint32_t SEC_PARAM_OOB { 0 }; /**< Out Of Band data not available. */
    static inline constexpr uint32_t SEC_PARAM_MIN_KEY_SIZE { 7 }; /**< Minimum encryption key size. */
    static inline constexpr uint32_t SEC_PARAM_MAX_KEY_SIZE { 16 }; /**< Maximum encryption key size. */
    
    
    /**
     * Initializes all relevant BLE code.
     * Initializes nRF's BLE stack, GAP and GATT info, services to communicate with 
     * electric skateboard (battery level, magnetic sensor info, serial comms?), and
     * the peer manager.
     */
    void init();
    
    /** 
      * Function for starting advertising. 
      * 
      * @param[in] p_erase_bonds Pointer to a boolean value representing whether or not
      *                          to erase BLE bonds. Is void * for compatibility with callbacks.
      */
    void advertising_start(void *p_erase_bonds);
    
    /**
     * Function for handling BLE events from the BSP module.
     *
     * @param[in] event Event generated by button press.
     */
    void bsp_event_handler(bsp_event_t event);
}

#endif // ESK8_BLE_PERIPHERAL_H