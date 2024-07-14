#ifndef _CALLBACK_H
#define _CALLBACK_H

#include <stdint.h>

#define MAX_CALLBACKS 10

typedef void (*callback_t)(void);

typedef struct {
    callback_t callback;
    uint32_t period_ticks;
    uint32_t last_called;
} callback_entry_t;

extern callback_entry_t callbacks[MAX_CALLBACKS];
extern uint32_t num_callbacks;

int register_callback(callback_t callback, uint32_t period_ticks);

#endif // _CALLBACK_H