#include "myTIM.h"
#include "myGPIO.h"
#include "myUART.h"

void TIM_period_us(uint32_t nTimer, float usec){
       
   if(nTimer == 1){   
	RCC->APB2ENR    |= RCC_APB2ENR_TIM1EN; 
	TIM1->PSC = usec*84 -1 ;                      // Timer counter clock: 10kHz(0.1ms)
	TIM1->ARR = 0xFFFF ;                           // Set auto reload register to maximum (count up to 65535)
	TIM1->CR1 &= ~(1<<4);                         // Up count
    TIM1->BDTR = TIM_BDTR_MOE;
   }
 if(nTimer == 2){   
	RCC->APB1ENR    |= RCC_APB1ENR_TIM2EN; 
	TIM2->PSC = usec*84 -1 ;                      // Timer counter clock: 10kHz(0.1ms)
	TIM2->ARR = 0xFFFF ;                           // Set auto reload register to maximum (count up to 65535)
	TIM2->CR1 &= ~(1<<4);                         // Up count
 }
 if(nTimer == 3){   
	RCC->APB1ENR    |= RCC_APB1ENR_TIM3EN; 
	TIM3->PSC = usec*84 -1 ;                      // Timer counter clock: 10kHz(0.1ms)
	TIM3->ARR = 0xFFFF ;                           // Set auto reload register to maximum (count up to 65535)
	TIM3->CR1 &= ~(1<<4);                         // Up count
 }
 if(nTimer == 4){   
	RCC->APB1ENR    |= RCC_APB1ENR_TIM4EN; 
	TIM4->PSC = usec*84 -1 ;                      // Timer counter clock: 10kHz(0.1ms)
	TIM4->ARR = 0xFFFF ;                           // Set auto reload register to maximum (count up to 65535)
	TIM4->CR1 &= ~(1<<4);                         // Up count
 }
 if(nTimer == 5){  
	RCC->APB1ENR    |= RCC_APB1ENR_TIM5EN; 
	TIM5->PSC = usec*84 -1 ;                      // Timer counter clock: 10kHz(0.1ms)
	TIM5->ARR = 0xFFFF ;                           // Set auto reload register to maximum (count up to 65535)
	TIM5->CR1 &= ~(1<<4);                         // Up count
 }
}


// Timer pinmap setting
void TIM_pinmap(TIM_t *timer_pin){
   
   if (timer_pin->port == GPIOA && (timer_pin->pin==5 || timer_pin->pin==0 || timer_pin->pin==15)){   
      timer_pin->timer = TIM2;
      timer_pin->ch = 1;   }
   else if (timer_pin->port == GPIOB && timer_pin->pin==10){
      timer_pin->timer = TIM2;
      timer_pin->ch = 3;   }
    else if((timer_pin->port == GPIOA && timer_pin->pin==1)||(timer_pin->port == GPIOB && timer_pin->pin==3)){
      timer_pin->timer = TIM2;
      timer_pin->ch = 2;   }
//--------------------------------------------------------------------------------------    
	if   ((timer_pin->port==GPIOA && (timer_pin->pin==6)) || ((timer_pin->port==GPIOB && (timer_pin->pin==4))) || ((timer_pin->port==GPIOC && (timer_pin->pin==6)))){   
      timer_pin->timer = TIM3;
      timer_pin->ch = 1;   }
	else if ((timer_pin->port==GPIOB && (timer_pin->pin==5)) || ((timer_pin->port==GPIOC && (timer_pin->pin==7))) ){
      timer_pin->timer = TIM3;
      timer_pin->ch = 2;   }
	else if(timer_pin->port == GPIOC && timer_pin->pin==8){
      timer_pin->timer = TIM3;
      timer_pin->ch = 3;   }
	else if(timer_pin->port == GPIOC && timer_pin->pin==9){
      timer_pin->timer = TIM3;
      timer_pin->ch = 4;   }
//--------------------------------------------------------------------------------------

	if(timer_pin->port == GPIOB && timer_pin->pin==6){
      timer_pin->timer = TIM4;
      timer_pin->ch = 1;   }
	else if(timer_pin->port == GPIOB && timer_pin->pin==7){
      timer_pin->timer = TIM4;
      timer_pin->ch = 2;   }
	else if(timer_pin->port == GPIOB && timer_pin->pin==8){
      timer_pin->timer = TIM4;
      timer_pin->ch = 3;   }
	else if(timer_pin->port == GPIOB && timer_pin->pin==9){
      timer_pin->timer = TIM4;
      timer_pin->ch = 4;   }
//--------------------------------------------------------------------------------------

	if(timer_pin->port == GPIOA && timer_pin->pin==8){
      timer_pin->timer = TIM1;
      timer_pin->ch = 1;   }
	else if(timer_pin->port == GPIOA && timer_pin->pin==9){
      timer_pin->timer = TIM1;
      timer_pin->ch = 2;   }
	else if(timer_pin->port == GPIOA && timer_pin->pin==10){
      timer_pin->timer = TIM1;
      timer_pin->ch = 3;   }

}




// Capturing pin setup configuration generalization
void CAP_init(TIM_t *cap_pin, GPIO_TypeDef *port, int pin){
	cap_pin->port = port;
    cap_pin->pin = pin;
   
    TIM_pinmap(cap_pin);
   // GPIO_afr(port, pin, cap_pin->timer);
   
   if (cap_pin->ch == 1){ 
    
      
		cap_pin->timer->CCMR1 &= ~(3);     
		cap_pin->timer->CCMR1 |= 1;                     // Capture/Compare Selection: CC1 is mapped on TI1
   
		cap_pin->timer->CCER  &= ~(1<<1);
		cap_pin->timer->CCER  &= ~(1<<2);
		cap_pin->timer->CCER  &= ~(1<<3);              
		cap_pin->timer->CCER  |=       1;                 // Capture rising edge // Capture enabled    
      
		cap_pin->timer->DIER  |= 1 ;                    // Interrupt enabled
		cap_pin->timer->DIER  |= 1 <<1;                 // Update interrupt enable   
      
		cap_pin->timer->CR1   |= 1;                     // Counter enable
   
      
       if(cap_pin->timer == TIM1){
       NVIC_SetPriority(TIM1_CC_IRQn, 2);            // Set the priority of TIM1 interrupt request
       NVIC_EnableIRQ( TIM1_CC_IRQn);                // TIM1 in terrupt request enable
       }
       if(cap_pin->timer == TIM2){
       NVIC_SetPriority(TIM2_IRQn, 2);               // Set the priority of TIM1 interrupt request
       NVIC_EnableIRQ( TIM2_IRQn);                   // TIM1 in terrupt request enable
       }
     if(cap_pin->timer == TIM3){
       NVIC_SetPriority(TIM3_IRQn, 2);               // Set the priority of TIM1 interrupt request
       NVIC_EnableIRQ( TIM3_IRQn);                   // TIM1 in terrupt request enable
       }
       if(cap_pin->timer == TIM4){
       NVIC_SetPriority(TIM4_IRQn, 2);               // Set the priority of TIM1 interrupt request
       NVIC_EnableIRQ( TIM4_IRQn);                   // TIM1 in terrupt request enable
       }
   }
   
    //----------------------------------------------------------------------------------------
   if (cap_pin->ch == 2){ 
		cap_pin->timer->CCMR1 &= ~(3<<8);     
		cap_pin->timer->CCMR1 |= 2<<8;                  // Capture/Compare Selection: CC2 is mapped on TI1
   
    cap_pin->timer->CCER  |= (1<<5);
		cap_pin->timer->CCER  &= ~(1<<7);   
		cap_pin->timer->CCER  &= ~(1<<6);          
		cap_pin->timer->CCER  |=    1<<4;                 // Capture falling edge // Capture enabled    
      
    cap_pin->timer->DIER  |= 1 ;                    // Interrupt enabled
		cap_pin->timer->DIER  |= 1 <<2;                 // Update interrupt enable   
      
    cap_pin->timer->CR1   |= 1;                     // Counter enable
   
      
       if(cap_pin->timer == TIM1){
       NVIC_SetPriority(TIM1_CC_IRQn, 2);            // Set the priority of TIM2 interrupt request
       NVIC_EnableIRQ( TIM1_CC_IRQn);                // TIM2 in terrupt request enable
       }
       if(cap_pin->timer == TIM2){
       NVIC_SetPriority(TIM2_IRQn, 2);               // Set the priority of TIM2 interrupt request
       NVIC_EnableIRQ( TIM2_IRQn);                   // TIM2 in terrupt request enable
       }
     if(cap_pin->timer == TIM3){
       NVIC_SetPriority(TIM3_IRQn, 2);               // Set the priority of TIM2 interrupt request
       NVIC_EnableIRQ( TIM3_IRQn);                   // TIM2 in terrupt request enable
       }
       if(cap_pin->timer == TIM4){
       NVIC_SetPriority(TIM4_IRQn, 2);               // Set the priority of TIM2 interrupt request
       NVIC_EnableIRQ( TIM4_IRQn);                   // TIM2 in terrupt request enable
       }
    }
    //----------------------------------------------------------------------------------------
    if (cap_pin->ch == 3){ 
       
		cap_pin->timer->CCMR2 &= ~(3);     
    cap_pin->timer->CCMR2 |= 1;                     // Capture/Compare Selection: CC3 is mapped on TI3
   
    cap_pin->timer->CCER  |= (1<<9) ;              
    cap_pin->timer->CCER  |= (1<<11) ;              
		cap_pin->timer->CCER  |=    1<<8;                 // Capture rising edge // Capture enabled    
      
    cap_pin->timer->DIER  |= 1 ;                    // Interrupt enabled
		cap_pin->timer->DIER  |= 1 <<3;                 // Update interrupt enable   
      
    cap_pin->timer->CR1   |= 1;                     // Counter enable
   
      
       if(cap_pin->timer == TIM1){
       NVIC_SetPriority(TIM1_CC_IRQn, 2);            // Set the priority of TIM3 interrupt request
       NVIC_EnableIRQ( TIM1_CC_IRQn);                // TIM3 in terrupt request enable
       }
       if(cap_pin->timer == TIM2){
       NVIC_SetPriority(TIM2_IRQn, 2);               // Set the priority of TIM3 interrupt request
       NVIC_EnableIRQ( TIM2_IRQn);                   // TIM3 in terrupt request enable
       }
     if(cap_pin->timer == TIM3){
       NVIC_SetPriority(TIM3_IRQn, 2);               // Set the priority of TIM3 interrupt request
       NVIC_EnableIRQ( TIM3_IRQn);                   // TIM3 in terrupt request enable
       }
       if(cap_pin->timer == TIM4){
       NVIC_SetPriority(TIM4_IRQn, 2);               // Set the priority of TIM3 interrupt request
       NVIC_EnableIRQ( TIM4_IRQn);                   // TIM3 in terrupt request enable
       }
    }
    //----------------------------------------------------------------------------------------
    if (cap_pin->ch == 4){ 
		cap_pin->timer->CCMR2 &= ~(3<<8);     
    cap_pin->timer->CCMR2 |= 2<<8;                   // Capture/Compare Selection: CC4 is mapped on TI3
   
    cap_pin->timer->CCER  |= (1<<13);
		cap_pin->timer->CCER  &= ~(1<<14);
		cap_pin->timer->CCER  &= ~(1<<15);       
		cap_pin->timer->CCER  |=    1<<12;                 // Capture falling edge // Capture enabled    
      
    cap_pin->timer->DIER  |= 1 ;                    // Interrupt enabled
		cap_pin->timer->DIER  |= 1 <<4;                 // Update interrupt enable   
      
    cap_pin->timer->CR1   |= 1;                     // Counter enable
   
      
       if(cap_pin->timer == TIM1){
       NVIC_SetPriority(TIM1_CC_IRQn, 2);            // Set the priority of TIM4 interrupt request
       NVIC_EnableIRQ( TIM1_CC_IRQn);                // TIM4 in terrupt request enable
       }
       if(cap_pin->timer == TIM2){
       NVIC_SetPriority(TIM2_IRQn, 2);               // Set the priority of TIM4 interrupt request
       NVIC_EnableIRQ( TIM2_IRQn);                   // TIM4 in terrupt request enable
       }
     if(cap_pin->timer == TIM3){
       NVIC_SetPriority(TIM3_IRQn, 2);               // Set the priority of TIM4 interrupt request
       NVIC_EnableIRQ( TIM3_IRQn);                   // TIM4 in terrupt request enable
       }
       if(cap_pin->timer == TIM4){
       NVIC_SetPriority(TIM4_IRQn, 2);               // Set the priority of TIM4 interrupt request
       NVIC_EnableIRQ( TIM4_IRQn);                   // TIM4 in terrupt request enable
       }
    }
}

int overflow =0;
float val=0;
float interval = 0;
float before=0;
float total = 0;
int distance1 = 0;
int distance2 = 0;


// Distance calculation from the first ultrasonic sensors 
void Distance1(TIM_t * cap_pin)
{
	if(cap_pin->timer->SR & TIM_SR_UIF)
	{ // Update interrupt
		overflow++ ;
		
		cap_pin->timer->SR &= ~(TIM_SR_UIF); 							// clear update interrupt flag
	}
	
	if((cap_pin->timer->SR & TIM_SR_CC1IF) != 0)
		{ 
			overflow=0 ;
			before = cap_pin->timer->CCR1;			
			cap_pin->timer->SR &= ~(TIM_SR_CC1IF);								// clear capture/compare interrupt flag ( it is also cleared by reading cap_pin->timer_CCR1)
		}
		
	if((cap_pin->timer->SR & TIM_SR_CC2IF) != 0)
		{ 
			val = cap_pin->timer->CCR2;				
			cap_pin->timer->SR &= ~(TIM_SR_CC2IF);								// clear capture/compare interrupt flag ( it is also cleared by reading cap_pin->timer_CCR1)				
			interval = val - before + overflow*0xFFFF; 			
			distance1	= (float)total/58;
			overflow=0 ;
		}	
	}

// Distance calculation from the second ultrasonic sensors 	
void Distance2(TIM_t * cap_pin)
{
	if(cap_pin->timer->SR & TIM_SR_UIF)
	{ // Update interrupt
		overflow++ ;
		
		cap_pin->timer->SR &= ~(TIM_SR_UIF); 							// clear update interrupt flag
	}
	
	if((cap_pin->timer->SR & TIM_SR_CC1IF) != 0)
		{ 
			overflow=0 ;
			before = cap_pin->timer->CCR1;			
			cap_pin->timer->SR &= ~(TIM_SR_CC1IF);								// clear capture/compare interrupt flag ( it is also cleared by reading cap_pin->timer_CCR1)
		}
		
	if((cap_pin->timer->SR & TIM_SR_CC2IF) != 0)
		{ 
			val = cap_pin->timer->CCR2;				
			cap_pin->timer->SR &= ~(TIM_SR_CC2IF);								// clear capture/compare interrupt flag ( it is also cleared by reading cap_pin->timer_CCR1)				
			interval = val - before + overflow*0xFFFF; 			
			distance2	= (float)total/58;
			overflow=0 ;
		}	
	}
