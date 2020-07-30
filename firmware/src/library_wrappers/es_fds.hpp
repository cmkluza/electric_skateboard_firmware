/*
 * es_fds.hpp - Functions to deal with the flash data storage library.
 *
 * Performs initialization of FDS and handles FDS events.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <fds.h>
#include <sdk_errors.h>

#include <cstdint>

namespace es_fds {

    /**
     * Initializes the FDS library.
     */
    void init();

    /**
     * Checks to see if the given record is stored in flash.
     *
     * @param[in] file_id the FDS file ID.
     * @param[in] record_key the FDS record key.
     * @param[out] desc the record descriptor filled in if the record is present.
     *
     * @return true if the record is present, else false.
     */
    bool record_is_present(std::uint16_t file_id, std::uint16_t record_key, fds_record_desc_t *desc);

    /**
     * Reads data from an FDS record.
     *
     * @param[in] desc the record descriptor to be read.
     * @param[out] buffer pointer to be written to.
     * @param[in] buffer_len size to be read.
     *
     * @return NRF_SUCCESS if the read was successful, or an error code.
     */
    ret_code_t read_record(fds_record_desc_t *desc, std::uint8_t *buffer, size_t buffer_len);

    /**
     * Performs the idle task for FDS.
     */
    void idle();

}  // namespace es_fds
