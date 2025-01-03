#include "Timer_Handler.h"
#include <stdio.h>  // added for NULL

static uint32_t timer_period = 0;

static Timer_Handler timer[MAX_HANDLER];

static uint8_t getFreeSlot(void);
static uint8_t getSlot(uint32_t period, Callbackfunction callbackfunction);

void timer_initialize(void)
{
    for(uint8_t i = 0; i < MAX_HANDLER; i++)
    {
        timer[i].timeout  = 0;
        timer[i].oldValue = 0;
        timer[i].isReadyToExecute = false;
        timer[i].isTimoutHandler  = false;
        timer[i].callbackfunction = NULL;
    }
}

void timer_setTimeoutHandler(uint32_t period, Callbackfunction callbackfunction)
{
    uint8_t index = timer_setHandler(period, callbackfunction);
    if(index < MAX_HANDLER)
    {
        timer[index].isTimoutHandler = true;
    }
}

uint8_t timer_setHandler(uint32_t period, Callbackfunction callbackfunction)
{
    uint8_t index = 0;
    if((callbackfunction != NULL) && (period > 0))
    {
        index = getFreeSlot();
        if(index < MAX_HANDLER)
        {
            timer[index].timeout  = period;
            timer[index].oldValue = timer_period;
            timer[index].isReadyToExecute = false;
            timer[index].isTimoutHandler  = false;
            timer[index].callbackfunction = callbackfunction;
        }    
    }
    return index;
}

void timer_stopHandler(uint32_t period, Callbackfunction callbackfunction)
{
    uint8_t index = 0;
    if(callbackfunction != NULL)
    {
        index = getSlot(period, callbackfunction);
        if(index < MAX_HANDLER )
        {
            timer[index].timeout  = 0;
            timer[index].oldValue = 0;
            timer[index].isReadyToExecute = false;
            timer[index].isTimoutHandler  = false;
            timer[index].callbackfunction = NULL;
        }  
    }
}

static uint8_t getFreeSlot(void)
{
    uint8_t index = MAX_HANDLER;
    for(uint8_t i = 0; i < MAX_HANDLER; i++)
    {
        if(timer[i].callbackfunction == NULL)
        {
            timer[i].timeout = 0xFFFFFFFF;
            index = i;
            break;
        }
    }
    
    return index;
}

static uint8_t getSlot(uint32_t period, Callbackfunction callbackfunction)
{
    uint8_t index = MAX_HANDLER;
    for(uint8_t i = 0; i < MAX_HANDLER; i++)
    {
        if((timer[i].timeout == period) && (timer[i].callbackfunction == callbackfunction))
        {
            index = i;
            break;
        }
    }
    
    return index;
}

void timer_handler(void)
{
    uint32_t period = 0;
    timer_period++;
    for(uint32_t i = 0; i < MAX_HANDLER; i++)
    {
        period = 0;
        if(timer[i].callbackfunction != NULL)
        {
            period = timer_period - timer[i].oldValue;
            if(period >= timer[i].timeout)
            {
                // high priority handler
                if(timer[i].isTimoutHandler)
                {
                    timer[i].oldValue = 0;   // reset
                    {
                        timer[i].callbackfunction();
                    }
                }

                // low priority handler
                else
                {
                    if(timer[i].isReadyToExecute == false)
                    {
                        timer[i].oldValue = timer_period;   // for next trigger
                        timer[i].isReadyToExecute = true;
                    }
                }
            }
        }
    }
}

void timer_run(void)
{
    for(uint32_t i = 0; i < MAX_HANDLER; i++)
    {
        if(timer[i].isReadyToExecute)
        {
            if(timer[i].callbackfunction != NULL)
            {
                timer[i].callbackfunction();
                timer[i].isReadyToExecute = false;
            }
        }
    }
}
