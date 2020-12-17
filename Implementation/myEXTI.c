#include "myEXTI.h"
float duty_em = 0.5;
int n1=0;
int dir=1;

// External interrupt setup configuration
void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority){
      
   RCC->AHB1ENR |=RCC_AHB1ENR_GPIOCEN;
   GPIOC->MODER &=~(3UL<<2*pin);//input :00
   GPIOC->PUPDR &=~(3UL<<2*pin); //No pupd
   NVIC_SetPriority(EXTI15_10_IRQn,0);
   
   RCC->APB2ENR |=RCC_APB2ENR_SYSCFGEN; //1<<14;
   
   SYSCFG->EXTICR[3] &=~(0xF <<4); //clear
   SYSCFG->EXTICR[3] |=priority<<4; //Port C
   
   EXTI->FTSR |= trig_type<<pin;
   
   EXTI->IMR|=1<<pin;
   
   NVIC_EnableIRQ(EXTI15_10_IRQn);   
}

// External interrupt handler for push button
void EXTI15_10_IRQHandler(){ 
   
   if(n1<11){      
      n1 = n1+1;}      
      if(n1==11){
      dir=dir*(-1);
         n1=1;
      }
   if ((EXTI->PR& EXTI_PR_PR13)==EXTI_PR_PR13){
   duty_em=duty_em+0.2*dir;
   EXTI->PR|=EXTI_PR_PR13;//clear method of this one
   }
}