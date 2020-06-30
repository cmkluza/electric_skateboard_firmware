/*
 * ble_esk8_server.cpp - Server for custom electric skateboard BLE service.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */
 
#include "ble_esk8_server.hpp"

#include <app_error.h>
#include <ble.h>
#include <ble_gatts.h>
#include <ble_srv_common.h>
#include <ble_types.h>


void BLEEsk8Server::init()
{
    /* Add vendor specific 128-bit UUID */
    ble_uuid128_t base_uuid = UUID_BASE;
    APP_ERROR_CHECK(sd_ble_uuid_vs_add(&base_uuid, &_uuid_type));
    
    /* Add custom electric skateboard service */
    ble_uuid_t uuid {
        .uuid = UUID_SERVICE,
        .type = _uuid_type,
    };
    APP_ERROR_CHECK(
        sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &uuid, &_service_handle));
    
    /* Add sensor data characteristic */
    uint8_t sensor_initial_value = {};
    ble_add_char_params_t add_char_params = {};
            
    /* Sensor characteristic */
    add_char_params.uuid                        = { UUID_SENSOR_CHAR };
    add_char_params.uuid_type                   = { _uuid_type };
    
    /* Fixed length, initialize to 0 */
    // TODO CMK 06/22/20: verify data type for sensor data
    add_char_params.max_len                     = { sizeof(uint8_t) };
    add_char_params.init_len                    = { add_char_params.max_len };
    add_char_params.p_init_value                = { &sensor_initial_value };
    add_char_params.is_var_len                  = { false };
    
    /* Read-only w/ notifications */
    add_char_params.char_props.broadcast        = { false };
    add_char_params.char_props.read             = { true };
    add_char_params.char_props.write_wo_resp    = { false };
    add_char_params.char_props.write            = { false };
    add_char_params.char_props.notify           = { true };
    add_char_params.char_props.indicate         = { false };
    add_char_params.char_props.auth_signed_wr   = { false };
    add_char_params.char_ext_props.reliable_wr  = { false };
    add_char_params.char_ext_props.wr_aux       = { false };
    
    /* No deferred operations */
    add_char_params.is_defered_read             = { false };
    add_char_params.is_defered_write            = { false };
            
    /* TODO CMK 06/22/20: security */
    add_char_params.read_access                 = { SEC_OPEN };
    add_char_params.write_access                = { SEC_NO_ACCESS }; // no writing
    add_char_params.cccd_write_access           = { SEC_OPEN };
    
    /* TODO CMK 06/22/20: once sensor moduel is developed - easier to use
                          local mem? */
    add_char_params.is_value_user               = { false };
    
    /* No characteristic user description descriptor */
    add_char_params.p_user_descr                = { nullptr };

    /* No characteristic presentation format descriptor */
    add_char_params.p_presentation_format       = { nullptr };
    
    APP_ERROR_CHECK(characteristic_add(_service_handle, &add_char_params, &_sensor_char_handles));
}

void BLEEsk8Server::event_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
    // TODO CMK 06/22/20: implement BLE event handler
}

void BLEEsk8Server::update_sensor_value(uint16_t conn_handle, uint8_t new_value)
{
    uint16_t len = { sizeof(new_value) };
    
    ble_gatts_hvx_params_t params = {
        .handle = _sensor_char_handles.value_handle,
        .type   = BLE_GATT_HVX_NOTIFICATION,
        .offset = 0,
        .p_len  = &len,
        .p_data = &new_value,
    };
        
    APP_ERROR_CHECK(sd_ble_gatts_hvx(conn_handle, &params));
}
