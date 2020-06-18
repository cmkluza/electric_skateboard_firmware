/*
 * util.hpp - miscellaneous utilities.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#ifndef ESK8_UTIL_H
#define ESK8_UTIL_H

namespace Util {
        
    /** 
     * Function for initializing the clock.
     */
    void clock_init();
    
    /**
     * Enables deep sleep instead of regular sleep in the SCB.
     */
    void enable_deep_sleep();

    /**
     * Function for putting the chip into sleep mode.
     *
     * @note This function will not return.
     */
    void enter_sleep_mode();

;}

#endif // ESK8_UTIL_H