#ifndef TIMER_IF_H
#define TIMER_IF_H
#include <stdint.h>
#include "timer_driver.h"

typedef struct 
{
    void (*tim_control_init)(void);
    void (*call_back_01)(TIM4_callback callback_01);
}timer_if_t;

extern const timer_if_t timer_STM32;

#endif
