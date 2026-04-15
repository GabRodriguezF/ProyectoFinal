#ifndef USART_IF_H
#define USART_IF_H
#include <stdint.h>
#include "usart_driver.h"

typedef struct 
{
    void(*init)(void);
    void(*w_byte)(uint8_t dato);
    uint8_t(*r_byte)(void);
    void (*call_back_01)(USART3_callback callback_01);
}usart3_f_t;

extern const usart3_f_t usart3_STM32;

#endif
