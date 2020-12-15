/*
 * es_fds.cpp - Functions to deal with the flash data storage library.
 *
 * Performs initialization of FDS and handles FDS events.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "es_fds.hpp"

#include <fds.h>
#include <nrf_strerror.h>

#include "logger.hpp"
using logger::Level;

namespace es_fds {

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Event handler for FDS events.
 *
 * @param[in] p_evt the FDS event.
 */
static void event_handler(fds_evt_t const *p_evt);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Whether or not the library is initialzied yet. */
static volatile bool g_is_initialized { false };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////
void init() {
    APP_ERROR_CHECK(fds_register(event_handler));
    APP_ERROR_CHECK(fds_init());

    while (!g_is_initialized) {
        /* Await initialization success */
    }
}

bool record_is_present(std::uint16_t file_id, std::uint16_t record_key, fds_record_desc_t *p_desc) {
    fds_find_token_t tok = {};
    auto ret_code = fds_record_find(file_id, record_key, p_desc, &tok);

    switch (ret_code) {
        case NRF_SUCCESS:
            return true;
        case FDS_ERR_NOT_FOUND:
            return false;
        default:
            APP_ERROR_HANDLER(ret_code);
    }

    return false;
}

ret_code_t read_record(fds_record_desc_t *desc, std::uint8_t *buffer, size_t buffer_len) {
    fds_flash_record_t config = {};

    if (auto ret_code = fds_record_open(desc, &config); ret_code != NRF_SUCCESS) {
        logger::log<Level::WARNING>("%s::fds_record_open failed: %s",
                                    __func__, nrf_strerror_get(ret_code));
        return ret_code;
    }

    memcpy(buffer, config.p_data, buffer_len);

    if (auto ret_code = fds_record_close(desc); ret_code != NRF_SUCCESS) {
        logger::log<Level::WARNING>("%s::fds_record_close failed: %s",
                                    __func__, nrf_strerror_get(ret_code));
        return ret_code;
    }

    return NRF_SUCCESS;
}

void idle() {
    // TODO(CMK) 06/29/20: implement garbage collection
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

static void event_handler(fds_evt_t const *p_evt) {
    if (NRF_SUCCESS == p_evt->result) {
        logger::log<Level::INFO>("%s successful event: %d", __FILE__, p_evt->id);
    } else {
        logger::log<Level::WARNING>("%s unsuccessful event: %d %s",
                                    __FILE__, p_evt->id, nrf_strerror_get(p_evt->result));
    }

    switch (p_evt->id) {
        case FDS_EVT_INIT:
            if (NRF_SUCCESS == p_evt->result) {
                g_is_initialized = true;
            } else {
                APP_ERROR_HANDLER(p_evt->result);
            }
            break;

        case FDS_EVT_WRITE:
            break;

        case FDS_EVT_UPDATE:
            break;

        case FDS_EVT_DEL_RECORD:
            break;

        case FDS_EVT_DEL_FILE:
            break;

        case FDS_EVT_GC:
            break;
    }
}

}  // namespace es_fds
