#include "myRCC.h"

void RCC_HSI_init()
{
	 RCC->CR |= RCC_CR_HSION; 
	 while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;} 
	 RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW)); 
	 RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;  
	 while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != 0 ) {;}  
}

	// To correctly read data from FLASH memory, the number of wait states (LATENCY)
  	// must be correctly programmed according to the frequency of the CPU clock
 	// (HCLK) and the supply voltage of the device.

void RCC_PLL_init(){
	FLASH->ACR &= ~FLASH_ACR_LATENCY; //59p
	FLASH->ACR |=  FLASH_ACR_LATENCY_2WS;
		
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	RCC->CR    &= ~RCC_CR_PLLON; //p.102
	
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; 		//initialize
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; //
	
	// Make PLL as 84 MHz
	// f(VCO clock) = f(PLL clock input) * (PLLN / PLLM) = 16MHz * 84/8 = 168 MHz
	// f(PLL_R) = f(VCO clock) / PLLP = 168MHz/2 = 84MHz
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | 84U << 6; // Initialization and Setting
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | 8U ; 
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;  // 00: PLLP = 2, 01: PLLP = 4, 10: PLLP = 6, 11: PLLP = 8	
	
	RCC->CR    |=  RCC_CR_PLLON; 
  	while((RCC->CR & RCC_CR_PLLRDY)>>25 != 0); // 0: unlock
	//p.106 Select PLL as system clock
	RCC->CFGR &= ~RCC_CFGR_SW;//
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	//wait until system clock
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != 8UL);
	
	RCC->CFGR &= ~RCC_CFGR_HPRE;  		
	RCC->CFGR &= ~RCC_CFGR_PPRE1; 		
	RCC->CFGR |=  RCC_CFGR_PPRE1_2;
	RCC->CFGR &= ~RCC_CFGR_PPRE2;
}