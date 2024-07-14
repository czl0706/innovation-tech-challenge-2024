#include "callback.h"

callback_entry_t callbacks[MAX_CALLBACKS];
uint32_t num_callbacks = 0;

int register_callback(callback_t callback, uint32_t period_ticks) {
    if (num_callbacks >= MAX_CALLBACKS) {
        return -1; // Callback array is full
    }

    callbacks[num_callbacks].callback = callback;
    callbacks[num_callbacks].period_ticks = period_ticks;
    callbacks[num_callbacks].last_called = 0;
    num_callbacks++;

    return 0; // Success
}