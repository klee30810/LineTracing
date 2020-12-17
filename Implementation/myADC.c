# include "myADC.h"
# include "myGPIO.h"

// ADC pinmapping generalization
uint32_t ADC_pinmap(GPIO_TypeDef *port, uint32_t pin)
{
   uint32_t channel = 0;
   if(port == GPIOA)
   {
      if(pin==0 || pin==1 || pin==4 || pin==5 || pin==6 || pin==7)
				channel = pin;
   }
   else if(port == GPIOB)
   {
		 if(pin==0 || pin==1)
				channel = pin+8;
   }
   else if(port == GPIOC)
   {
      if(pin==0 || pin==1 || pin==2 || pin==3 || pin==4 || pin==5)
				channel = pin+10;
   }
   
   return channel;
}


// ADC setup configuration
void ADC_init(GPIO_TypeDef *port, uint32_t pin)
{
		GPIO_init(port,pin,ANALOG);
		uint32_t ch = ADC_pinmap(port, pin);
   
   
      // 1. enable ADC peripheral clock
   RCC->APB2ENR    |= RCC_APB2ENR_ADC1EN;
   
      // 2. configure ADC clock pre-scaler
   ADC->CCR           &= ~ADC_CCR_ADCPRE;

      // 3. configure ADC resolution
   ADC1->CR1 					&= ~ADC_CR1_RES; 
   
      // 4. configure ADC data alignment
   ADC1->CR2       &= ~(ADC_CR2_ALIGN);
   
      // 5. enable continuous conversion mode
   ADC1->CR2       &= ~ADC_CR2_CONT;
	
   ADC1->CR1 |= ADC_CR1_SCAN; //scan mode

      // 6. Configure the sequence length 
   ADC1->SQR1       &= ~(15<<20);
   ADC1->SQR1       |= (1<<20);  //conversion 2°³·Î ´Ã¸²
   
      // 7. Configure the channel sequence 
   ADC1->SQR3       &= ~(31);
   ADC1->SQR3       &= ~(31<<ch*5);
	 ADC1->SQR3       |= (ch<<5*ch);
	 
	    
      // 8. Configure channel sampling time of conversion
  	if((ch>=10) && (ch<=16)){
		ADC1->SMPR2 &= ~(7 << 3*(ch%10));   
		ADC1->SMPR2 |= (4 << 3*(ch%10));					// 84cycles
	}
		else if(ch<=9){
		ADC1->SMPR2 &= ~(7 << 3*(ch%10));   
		ADC1->SMPR2 |= (4 << 3*(ch%10));					// 84cycles	
	}


      // 9. Enable ADC  ADC1_CR2 -> ADON
   ADC1->CR2         |= ADC_CR2_ADON;
   
      // 10. Start conversion ADC1_CR2 -> SWSTART
   ADC1->CR2         |= ADC_CR2_SWSTART;
   
      // 11. EOC interrupt on
	 ADC1->CR2 |= ADC_CR2_EOCS;
   ADC1->CR1       |= ADC_CR1_EOCIE;
   
	NVIC_SetPriority(ADC_IRQn, 0);               // Set the priority of TIM1 interrupt request
  NVIC_EnableIRQ(ADC_IRQn); 
}

// ADC trigger output configuration
void ADC_init_TRGO(GPIO_TypeDef *port, uint32_t pin)
{
		GPIO_init(port,pin,ANALOG);
		GPIO_pupd(port,pin,GPIO_PUPD_NPUPD);
		
   
   
      //enable ADC peripheral clock
		RCC->APB2ENR    |= RCC_APB2ENR_ADC1EN;
//		uint32_t ch = ADC_pinmap(port, pin);   //Pin mapping
		ADC_init(port, pin);


    	
//		// Select Timer 3 OC1REF as TRGO
//		TIM3->CR2 &=~TIM_CR2_MMS;
//		TIM3->CR2 |= TIM_CR2_MMS_2; 	// OC1REF Compare 
//	
//		// TIM3 TRGO Event as External Trigger
//		ADC1 -> CR2 &= ~ADC_CR2_EXTSEL;
//		ADC1 -> CR2 |= ADC_CR2_EXTSEL_3; 
//	
//		// TIM3 TRGO as rising event
//		ADC1 -> CR2 &= ~ADC_CR2_EXTEN;
//		ADC1 -> CR2 |= ADC_CR2_EXTEN_0; // rising edge
		
//		// Select Timer 5 OC1REF as TRGO
//		TIM9->CR2 &=~TIM_CR2_MMS;
//		TIM9->CR2 |= TIM_CR2_MMS_2; 	// OC1REF Compare 
//	
//		// TIM5 TRGO Event as External Trigger
//		ADC1 -> CR2 &= ~ADC_CR2_JEXTSEL;
//		ADC1 -> CR2 |= (11<<ADC_CR2_JEXTSEL_Pos); 
//	
//		// TIM5 TRGO as rising event
//		ADC1 -> CR2 &= ~ADC_CR2_JEXTEN;
//		ADC1 -> CR2 |= ADC_CR2_JEXTEN_0; // rising edge	
	
		ADC1->CR1 |= ADC_CR1_EOCIE;   // Interrup enable
		ADC1->CR2 |= ADC_CR2_EOCS;    // Enable interrupt 
	
		//ADC1->CR2 |= ADC_CR2_EXTEN_0;
		//ADC1->CR2 |= ADC_CR2_JEXTEN_0;
		
		ADC1->CR2 |= ADC_CR2_ADON;						// ADC_CR2 -> ADON: 1
		ADC1->CR2 |= ADC_CR2_SWSTART;						// ADC_CR2 -> SWSTART: 1 
	
		NVIC_SetPriority(ADC_IRQn, 1);
		NVIC_EnableIRQ(ADC_IRQn);			
}

// ADC sequence number configuration
void ADC_sequence_num(uint32_t num)
{
	ADC1->SQR1  &= ~(15 << 20);								
	ADC1->SQR1	|=	((num - 1) << 20);
}

// ADC order setup
void ADC_order(GPIO_TypeDef* port, uint32_t pin, uint32_t order)
{
	uint32_t ch = ADC_pinmap(port, pin);

	ADC1->SQR3	&=	~(31 << (5 * order));
	ADC1->SQR3	|=	ch << (5 * order);
}

// ADC_conversion start!
void ADC_convert()
{
	ADC1->CR2 |= (1 <<30); //start conversion
}

// read Analog input
uint32_t ADC_read()
{
	uint32_t result = 0;
	
	while((ADC1->SR & (1 << 1)) == 0){} 

	result = ADC1->DR; 

	return result;
}

// Timer 3 setting for ADC
void TIM3_init(float msec){
	if (msec>1000 || msec<0)
		msec = 100;
	
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
		//Up-counter
		TIM3->CR1 &=~TIM_CR1_DIR;  
	
		//OC1M = 110 for PWM Mode 1 output on ch1
		TIM3->CCMR1 &=~TIM_CCMR1_OC1M;
		TIM3->CCMR1 |= (6<<TIM_CCMR1_OC1M_Pos);
	
		// CC1 Capture mode enabled
		TIM3->CCER |= TIM_CCER_CC1E;             
		TIM3->CCER |= ~((TIM_CCER_CC1NP) | (TIM_CCER_CC1P) ); // rising edge
		TIM3->CCR1 = (10*msec)-1;

		// OC1REF used as trigger output	
		TIM3->CR2&= ~TIM_CR2_MMS;		
		TIM3->CR2|= TIM_CR2_MMS_2;			
	
		// Configure Timer Period	
		TIM3->PSC = 83999;
		TIM3->ARR=(int)(10*msec)-1;
	
		// OC1 Signal is Output
		TIM3->CCER |=TIM_CCER_CC1E;
	
		// Enable Timer
		TIM3->CR1|=TIM_CR1_CEN;
}