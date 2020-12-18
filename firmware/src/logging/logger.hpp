/*
 * logger.hpp - logging abstraction.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstddef>

namespace logger {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////////////////////////
enum class Level {
    DBG,      /**< For verbose debugging info */
    INFO,       /**< For general info */
    WARNING,    /**< For erraneous behavior that can be recovered from */
    ERROR       /**< For erraneous behavior that cannot be recovered from */
};

enum class Option {
    DEFAULT,    /**< Default logging operation */
    NO_HEADER,  /**< Do not print a header in front of the log */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the logger.
 */
void init();

/**
 * Perform an idle task for the logger if there is one.
 */
void idle();

/**
 * Log info at a given level. Specializations per log level are in platform-specific header.
 */
template<Level level, Option ... options, typename ... Args>
void log(const char *fmt, Args ... args);

/**
 * Log a hexdump of bytes.
 */
template<Level level>
void hexdump(const void *ptr, std::size_t len);

/**
 * Push a temporary string into a buffer for later use if logger is deferred.
 */
char const *push(char *str);

}  // namespace logger

/**
 * Include any implementation specific headers.
 */
#ifdef NRF52840_XXAA
/* Logger header implementation with nRF specific implementation. */
#   include "logger_nrf_log.hpp"
#else
#   warning "Application specific logging header(s) not found"
#endif
