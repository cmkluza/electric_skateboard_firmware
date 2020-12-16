/*
 * logger_nrf_log.hpp - logging implementation that passes through to nRF's log library and
 *                      FreeRTOS.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <nrf_log.h>

#include <cstddef>
#include <cstdint>

namespace logger {

/**
 * Check if an option is in the template parameter pack.
 *
 * @ret true if the given argument is in the template pack, else false.
 */
template<Option ... options>
static constexpr bool is_in_options(Option option) {
    constexpr Option opts[] = { options... };
    constexpr std::size_t opt_cnt = sizeof...(options);

    for (std::size_t i = 0; i < opt_cnt; ++i) {
        if (opts[i] == option) {
            return true;
        }
    }

    return false;
}

/**
 * Based on the log level and options return the severity as used by the SDK's logger.
 *
 * @ret the severity level as used by the SDK.
 */
template<Level level, Option ... options>
static constexpr std::uint32_t get_severity() {
    /* In nRF SDK, no header is only applicable to info level. */
    if constexpr (is_in_options<options...>(Option::NO_HEADER)) {
        return NRF_LOG_SEVERITY_INFO_RAW;
    }

    if constexpr (level == Level::DEBUG) {
        return NRF_LOG_SEVERITY_DEBUG;
    } else if constexpr (level == Level::INFO) {
        return NRF_LOG_SEVERITY_INFO;
    } else if constexpr (level == Level::WARNING) {
        return NRF_LOG_SEVERITY_WARNING;
    } else /* if constexpr (level == Level::ERROR) */ {
        return NRF_LOG_SEVERITY_ERROR;
    }
}

/**
 * Checks if logs should happen based on compile-time logging levels and filters. Uses
 * checks from Nordic's "NRF_LOG_INTERNAL_INST" macro.
 *
 * @ret true if logging is enabled, else false.
 */
template <std::uint32_t level>
static constexpr bool is_log_enabled() {
    if constexpr (NRF_LOG_ENABLED && (NRF_LOG_LEVEL >= level) && (level <= NRF_LOG_DEFAULT_LEVEL)) {
        if constexpr (NRF_LOG_FILTER >= level) {
            return true;
        }
    }

    return false;
}

/**
 * Implement the log template using nRF SDK's logger.
 * Note: Requires somewhat hacky usage of nRF logger internal functions
 *       instead of hte "NRF_LOG_<level>" macros, which don't play nicely
 *       with templates like this one.
 */
template<Level level, Option ... options, typename ... Args>
void log(const char *fmt, Args ... args) {
    constexpr std::size_t arg_cnt = sizeof...(args);
    constexpr std::uint32_t severity = get_severity<level, options...>();

    static_assert(arg_cnt < 7, "Number of arguments must be less than 7");

    if constexpr (!is_log_enabled<severity>()) {
        /* Logging not enabled - do nothing */
    } else if constexpr (arg_cnt == 0) {
        nrf_log_frontend_std_0(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    } else if constexpr (arg_cnt == 1) {
        nrf_log_frontend_std_1(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    } else if constexpr (arg_cnt == 2) {
        nrf_log_frontend_std_2(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    } else if constexpr (arg_cnt == 3) {
        nrf_log_frontend_std_3(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    } else if constexpr (arg_cnt == 4) {
        nrf_log_frontend_std_4(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    } else if constexpr (arg_cnt == 5) {
        nrf_log_frontend_std_5(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    } else if constexpr (arg_cnt == 6) {
        nrf_log_frontend_std_6(LOG_SEVERITY_MOD_ID(severity), fmt, ((std::uint32_t) args)...);
    }
}

/**
 * Pass through to nRF SDK's macro.
 */
template<Level level>
void hexdump(const void *ptr, std::size_t len) {
    constexpr std::uint32_t severity = get_severity<level>();

    if constexpr (is_log_enabled<severity>()) {
        LOG_HEXDUMP(LOG_SEVERITY_MOD_ID(severity), ptr, len);
    }
}

/**
 * Pass through to nRF SDK's macro.
 */
inline char const *push(char *str) {
    if constexpr (NRF_LOG_ENABLED) {
        return NRF_LOG_PUSH(str);
    }
}

}  // namespace logger
