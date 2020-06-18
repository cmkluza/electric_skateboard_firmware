/*
 * logger.hpp - utilities for logging.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#ifndef ESK8_LOGGER_H
#define ESK8_LOGGER_H

namespace Logger {
    /**
     * Initializes nRF's logger and its backends.
     */
    void init();
    
    /**
     * Performs the idle task for the logger.
     */
    void idle();
}

#endif // ESK8_LOGGER_H