#include "stm32f4xx.h"
#include <stdint.h>

#ifndef ADC_HEADER
#define ADC_HEADER


void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4);
void Stepper_setSpeed(uint32_t speed);
void Stepper_step (int steps_to_move);
void Stepper_stepMotor (uint32_t sequence);





#endif