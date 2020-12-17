#include "stm32f411xe.h"
#include "myRCC.h"
#include "myGPIO.h"
#include "mySysTick.h"

#ifndef EXTI_HEADER
#define EXTI_HEADER
#define BUTTON_PIN 13
#define FALL 1


void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority);
void EXTI15_10_IRQHandler();

#endif