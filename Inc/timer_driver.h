#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

void timer4_init(void);

typedef void(*TIM4_callback)(void);
void register_TIM4_callback1(TIM4_callback c);


#endif /* TIMER_DRIVER_H */
