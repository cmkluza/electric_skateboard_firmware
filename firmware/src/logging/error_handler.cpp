/*
 * error_handler.cpp - utilities for handling errors.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "error_handler.hpp"

#include <hardfault.h>
#include <nrf52.h>

// TODO CMK 07/01/20: call NRF_LOG_FINAL_FLUSH as needed

// TODO CMK 06/19/20: implement hard fault error handling
extern "C" 
void HardFault_process(HardFault_stack_t *p_stack)
{
    NVIC_SystemReset();
}

#if 0 // TODO CMK 06/19/20: implement app error handling
extern "C"
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{

}
#endif
