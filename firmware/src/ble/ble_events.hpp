/*
 * ble_events.hpp - BLE event types and callbacks.
 *
 * Defines BLE event types and maintains a list of BLE event callbacks that the app may register and
 * the BLE modules may trigger.
 *
 * Copyright (c) 2020 Cameron Kluza
 * Distributed under the MIT license (see LICENSE or https://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstdint>

namespace ble_events {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Types of BLE events the app can get alerts for.
 */
enum class Events {
    CONNECTED,      /**< Connected to a BLE device. */
    DISCONNECTED,   /**< Disconnected from a BLE device. */
    CCCD_WRITE,     /**< The CCCD for the sensor char was updated. */
    COUNT,          /**< For declaring arrays. */
};

/**
 * Data for the BLE events the app can get alerts for.
 */
union EventData {
    struct Connected {
        const std::uint8_t *address; /**< Pointer to the 6-byte BLE address. */
    } connected;
    struct Disconnected {
        const std::uint8_t *address; /**< Pointer to the 6-byte BLE address. */
        std::uint8_t reason;         /**< Reason for disconnect. */
    } disconnected;
    struct CCCDWrite {
        bool notifications_enabled; /**< Whether or not notifications are enabled. */
    } cccd_write;
};

/**
 * Struct handed to the app when a registered event occurs.
 */
struct Event {
    Events event;
    EventData data;
};

/**< The type of an event callback. */
using EventCallback = void (*)(Event *event);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Registers an event callback for the app.
 *
 * Pass nullptr to unregister for the event.
 *
 * @param[in] event    the event type to register for.
 * @param[in] callback the function to call when the event occurs.
 */
void register_event(Events event, EventCallback callback);

/**
 * Calls the callback for the given event with the given data, if one is registered.
 *
 * @param[in] event the event object.
 */
void trigger_event(Event *event);

}  // namespace ble_events
