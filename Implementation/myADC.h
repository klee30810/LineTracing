#ifndef _myADC_H
#define _myADC_H

#include	"stm32f4xx.h"
#include	"stm32f411xe.h"

#define ADC_PIN 0
#define INT 3


uint32_t ADC_pinmap(GPIO_TypeDef *port, uint32_t pin); 
void ADC_init(GPIO_TypeDef *port, uint32_t pin); 
uint32_t ADC_read();
void ADC_sequence_num(uint32_t num); 
void ADC_order(GPIO_TypeDef* port, uint32_t pin, uint32_t order); 
void ADC_convert();

void ADC_init_TRGO(GPIO_TypeDef* port, uint32_t pin);
void TIM3_init(float msec);
#endif

