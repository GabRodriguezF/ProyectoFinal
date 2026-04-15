#include "usart_driver.h"
#include "usart_if.h"

const usart3_f_t usart3_STM32 ={
    .init = usart_init,
    .r_byte = read_byte,
    .w_byte = write_byte,
	.call_back_01 = register_USART3_callback1
};
