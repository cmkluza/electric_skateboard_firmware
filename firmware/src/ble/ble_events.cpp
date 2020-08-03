/*
 * ble_events.cpp - BLE event types and callbacks.
 *
 * Defines BLE event types and maintains a list of BLE event callbacks that the app may register and
 * the BLE modules may trigger.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#include "ble_events.hpp"

#include <nrf_assert.h>
#include <nordic_common.h>

namespace ble_events {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Data
////////////////////////////////////////////////////////////////////////////////////////////////////

/**< Array of event callbacks. */
static EventCallback g_callbacks[static_cast<unsigned>(Events::COUNT)] = {};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Implementations
////////////////////////////////////////////////////////////////////////////////////////////////////

void register_event(Events event, EventCallback callback) {
    auto idx = static_cast<unsigned>(event);
    ASSERT(idx < ARRAY_SIZE(g_callbacks));

    g_callbacks[idx] = callback;
}

void trigger_event(Event *event) {
    ASSERT(event != nullptr);

    auto idx = static_cast<unsigned>(event->event);
    ASSERT(idx < ARRAY_SIZE(g_callbacks));

    if (g_callbacks[idx]) {
        g_callbacks[idx](event);
    }
}

}  // namespace ble_events
