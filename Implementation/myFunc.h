#include "myTIM.h"

#ifndef MY_FUNC_H
#define MY_FUNC_H


#include "myHEADER.h"



void timer_init(TIM_t *cap_pin, float msec);
void timer_setup(TIM_t *tim_int, TIM_TypeDef *TIMx, int TIM_ch, int msec);

// Wheel
void wheel_setup(PWM_t *pwm1, GPIO_TypeDef *port1, int pin1, PWM_t *pwm2, GPIO_TypeDef *port2, 	int pin2);

// Wheel control
void wheel(PWM_t *pwm1, PWM_t *pwm2, int direction, int speed);


// Ultrasonic
void Ultrasonic_setup();


// IR sensor
void IRsensor_setup();



#endif