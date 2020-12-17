#include "mySysTick.h"

uint32_t msTicks;
uint32_t Tim4Ticks; 


void SysTick_init(void){	
	//  SysTick Control and Status Register
	SysTick->CTRL = 0;										// Disable SysTick IRQ and SysTick Counter
	
	// SysTick Reload Value Register
	SysTick->LOAD = 84000000/1000 - 1;    // 1ms, Default clock
	
	// SysTick Current Value Register
	SysTick->VAL = 0;

	NVIC_SetPriority(SysTick_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);					// Enable interrupt in NVIC

	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
}

void SysTick_Handler(void){
	SysTick_counter();	
}

void SysTick_counter(){
	msTicks++;
}	

void delay (uint32_t T){
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < T);
	
	msTicks = 0;
}
/////////////TIM4 counter///////////////

void Timer_init(){
	uint32_t pre;
	TIM4->CR1       &= ~(1<<4);
	TIM4->CR1       |= (1<<7);             // Counting direction: 0 = up-counting, 1 = down-counting

	TIM4->PSC = 83;
	TIM4->CNT = 84000000/(83+1);
	TIM4-> ARR = (int)(1000-1);
	
	TIM4->CCMR1   &= ~TIM_CCMR1_OC1M;    // Clear ouput compare mode bits for channel 1
	TIM4->CCMR1   |= (1<<4);
	
  TIM4->CCER    &= ~TIM_CCER_CC1P;      // select active high    
	TIM4->CCER    |= TIM_CCER_CC1E;       // Enable output for CH1
	
	TIM4->CR1    |= TIM_CR1_CEN;         // Enable counter
	
}

void delay_ms (uint32_t msec){
  uint32_t curTim4Ticks;

  curTim4Ticks = (TIM4->CCR1);
  while (((TIM4->CCR1) -curTim4Ticks ) < msec);
	
	msTicks = 0;
}


