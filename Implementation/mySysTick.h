#ifndef __MY_SYSTICK_H
#define __MY_SYSTICK_H

#include "stm32f411xe.h"
#include <stdint.h>

void SysTick_init(void);
void SysTick_Handler(void);
void SysTick_counter();
void delay (uint32_t T);
void delay_ms (uint32_t msec);


typedef struct {

TIM_TypeDef *timer;

} Timer_t;

#endif