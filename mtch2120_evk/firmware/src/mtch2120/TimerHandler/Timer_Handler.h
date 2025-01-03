#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

typedef void(*Callbackfunction)(void);

typedef struct
{
    uint32_t timeout;
    uint32_t oldValue;
    bool isTimoutHandler;
    bool isReadyToExecute;
    Callbackfunction callbackfunction;
}Timer_Handler;

#define MAX_HANDLER     5u

void timer_initialize(void);

void timer_setTimeoutHandler(uint32_t period, Callbackfunction callbackfunction);
uint8_t timer_setHandler(uint32_t period, Callbackfunction callbackfunction);
void timer_stopHandler(uint32_t period, Callbackfunction callbackfunction);
void timer_run(void);

void timer_handler(void);

#endif // TIMER_H
