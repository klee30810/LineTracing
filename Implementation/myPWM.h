#ifndef _myPWM_H
#define _myPWM_H

#include "stm32f411xe.h"

typedef struct {
GPIO_TypeDef *port;
int pin;
TIM_TypeDef *timer;
int ch;
} PWM_t;


void PWM_init(PWM_t *pwm, GPIO_TypeDef *Port, int pin);

void PWM_pinmap(PWM_t *pwm);

void PWM_period_ms(PWM_t *pwm, float period_ms);

void PWM_pulsewidth_ms(PWM_t *pwm, float pulsewidth_ms);

void PWM_duty (PWM_t *pwm, float duty);

void TIM2_IRQHandler(void);

#endif