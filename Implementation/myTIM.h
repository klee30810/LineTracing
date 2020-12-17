#ifndef _myTIM_H
#define _myTIM_H

#include "stm32f411xe.h"

typedef struct {
GPIO_TypeDef *port;
int pin;
TIM_TypeDef *timer;
int ch;
} TIM_t;



void TIM_period_us(uint32_t nTimer, float usec);
void TIM_pinmap(TIM_t *timer_pin);
void CAP_init(TIM_t *cap_pin, GPIO_TypeDef *port, int pin);
void Distance1(TIM_t * cap_pin);
void Distance2(TIM_t * cap_pin);

#endif