/*
 * ble_central.hpp - BLE central module.
 *
 * Performs initialization and maintenance of various BLE central functionalities.
 *
 * The electric skatebaord remote shall act primarily as a BLE central. It will find and connect to
 * the electric skateboard receiver, and serve sensor data to it so it can set speed accordingly.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once
 
#include "ble_common.hpp"
 
#include <sdk_errors.h>
 
namespace ble_central {

     
////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
     
    /**
     * Initializes BLE central modules.
     *
     * TODO CMK 06/24/20: update with specifics
     *
     * @return NRF_SUCCESS if successfully initialized, otherwise an error code indicating 
     *         an unrecoverable issue occurred.
     */
    ret_code_t init(const ble_common::Config &config);
    
    /**
     * Sets the filter for the scanning module.
     */
    
    /**
     * Begins scanning for BLE devices.
     */
    void begin_scanning();
    
} // namespace ble_central
