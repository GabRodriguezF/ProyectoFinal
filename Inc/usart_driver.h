#ifndef USART_DRIVER_H
#define USART_DRIVER_H
#include <stdint.h>

void usart_init(void);
void write_byte(uint8_t d);
uint8_t read_byte(void);

//typedef void(*USART3_callback)(void);
//void register_USART3_callback1(USART3_callback c);

#endif
