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

#include <ble_gap.h>
#include <ble_types.h>
#include <sdk_errors.h>

#include <cstdint>
 
namespace ble_central {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
     
    /**
     * Initializes BLE central modules.
     *
     * TODO CMK 06/24/20: update with specifics
     */
    void init(const ble_common::Config &config);
    
    /**
     * Sets the filter for the scanning module.
     *
     * @param[in] addr reference to the BLE address to filter for.
     */
    void set_addr_scan_filter(const ble_gap_addr_t &addr);
    
    /**
     * Sets the filter for the scanning module.
     *
     * @param[in] uuid reference to the BLE UUID to filter for.
     * @param[in] appearance the BLE appearance to filter for.
     */
    void set_uuid_appearance_scan_filter(const ble_uuid_t &uuid, std::uint16_t appearance);
    
    /**
     * Begins scanning for BLE devices.
     */
    void begin_scanning();
    
} // namespace ble_central
