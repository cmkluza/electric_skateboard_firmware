/*
 * logger.hpp - utilities for logging.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <sdk_errors.h>

namespace logger {
    /**
     * Initializes nRF's logger and its backends.
     */
    void init();
    
    /**
     * Performs the idle task for the logger.
     */
    void idle();
}