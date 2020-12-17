#include "myHEADER.h"
int msec = 0;

// General Timer preparation for setup
void TIM_init(TIM_t *cap_pin, float msec)
{
	if (cap_pin->timer == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // assign appropriate RCC from timers
	if (cap_pin->timer == TIM9) RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	if (cap_pin->timer == TIM10) RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	if (cap_pin->timer == TIM11) RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	if (cap_pin->timer == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	if (cap_pin->timer == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	if (cap_pin->timer == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	if (cap_pin->timer == TIM5) RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	cap_pin->timer->CR1 &= ~(TIM_CR1_DIR); 		// upcounting

	cap_pin->timer->CCMR1 &= ~(TIM_CCMR1_OC1M_0|TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2);
	cap_pin->timer->CCMR1 |= TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2;
	cap_pin->timer->CCMR1 |= TIM_CCMR1_OC1PE; // CCMR setting for pwm execution

	cap_pin->timer->CR2&= ~(TIM_CR2_MMS);			// master mode
	cap_pin->timer->CR2|= TIM_CR2_MMS_2;			// trigger output

	cap_pin->timer->PSC = 8400 * 5 - 1;         
	cap_pin->timer->ARR = 2 * msec - 1;					

	cap_pin->timer->CCER &= ~(TIM_CCER_CC1P|TIM_CCER_CC1NP) ;     // CC1 capture rising edge  
	cap_pin->timer->CCER |= TIM_CCER_CC1E;												// CC1 enable

	cap_pin->timer->CCR1 = msec;																

	cap_pin->timer->DIER |= TIM_DIER_UIE; 							// interrupt enable	

	if (cap_pin->ch == 1) 	cap_pin->timer->DIER |= TIM_DIER_CC1IE; 						// interrupt enable	CC1	
	if (cap_pin->ch == 2) 	cap_pin->timer->DIER |= TIM_DIER_CC2IE; 						// interrupt enable	CC2	
	if (cap_pin->ch == 3) 	cap_pin->timer->DIER |= TIM_DIER_CC3IE; 						// interrupt enable	CC3	
	if (cap_pin->ch == 4) 	cap_pin->timer->DIER |= TIM_DIER_CC4IE; 						// interrupt enable	CC4	

	cap_pin->timer->CR1 |= TIM_CR1_CEN;														// counter enable

}

// General timer setup configuration
void timer_setup(TIM_t *tim_int, TIM_TypeDef *TIMx, int TIM_ch, int msec)			// general timer setup
{
	tim_int->timer = TIMx;
	tim_int->ch = TIM_ch;

	TIM_init(tim_int, msec);			// 10[ms] period for assigned timer

}

// Wheel setup configuration
void wheel_setup(PWM_t *pwm1, GPIO_TypeDef *port1, int pin1, PWM_t *pwm2, GPIO_TypeDef *port2, 	int pin2){
	// pwm1	
	PWM_init(pwm1, port1, pin1);
	PWM_period_ms(pwm1, 25);													// 25[ms], 40[Hz]
	PWM_duty(pwm1, 0);

	// pwm2
	PWM_init(pwm2, port2, pin2);
	PWM_period_ms(pwm2, 25);													// 25[ms], 40[Hz]
	PWM_duty(pwm2, 0);
}

// Wheel control with the direction and speed from duty adjustment
void wheel(PWM_t *pwm1, PWM_t *pwm2, int direction, int speed) {		
	if (direction == 0) {			// stop
		PWM_duty(pwm1, 0);
		PWM_duty(pwm2, 0);
	}
	else if (direction == 1) {	// forward
		PWM_duty(pwm1, 0);
		PWM_duty(pwm2, speed);
	}
	else if (direction == 2) {	// backward 
		PWM_duty(pwm1, speed);
		PWM_duty(pwm2, 0);
	}
}


// Ultrasonic sensor setting configuration
void Ultrasonic_setup() {					

	// timer2
	if (msec > 1000 || msec < 0)
		msec = 100;

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;				// TIM2 RCC

	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;					// Timer mode setting
	TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	TIM2->PSC = 8400 * 5 - 1;
	TIM2->ARR = (2 * msec) - 1;

	TIM2->CCER |= ~((TIM_CCER_CC1NP) | (TIM_CCER_CC1P)); // rising edge
	TIM2->CCER |= TIM_CCER_CC1P;
	TIM2->CCER |= TIM_CCER_CC1E;

	TIM2->CCR1 = (10 * msec) - 1;

	TIM2->CR1 |= TIM_CR1_CEN;

	TIM2->DIER |= TIM_DIER_CC1IE;				// CC1 Capture compare enable
	TIM2->DIER |= TIM_DIER_UIE;					// Interrupt update enable

	NVIC_SetPriority(TIM2_IRQn, 2);            
	NVIC_EnableIRQ(TIM2_IRQn);

	// timer4
	if (msec > 1000 || msec < 0)
		msec = 100;

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;			// TIM4 RCC

	TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;				// Timer mode setting
	TIM4->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
	TIM4->CCMR1 |= TIM_CCMR1_OC1PE;

	TIM4->PSC = 8400 * 5 - 1;
	TIM4->ARR = (2 * msec) - 1;

	TIM4->CCER |= ~((TIM_CCER_CC1NP) | (TIM_CCER_CC1P)); // rising edge
	TIM4->CCER |= TIM_CCER_CC1P;
	TIM4->CCER |= TIM_CCER_CC1E;

	TIM4->CCR1 = (10 * msec) - 1;

	TIM4->CR1 |= TIM_CR1_CEN;

	TIM4->DIER |= TIM_DIER_CC1IE;				// CC1 Capture compare enable
	TIM4->DIER |= TIM_DIER_UIE;					// Interrupt update enable

	NVIC_SetPriority(TIM4_IRQn, 2);            // Set the priority of TIM4 interrupt request
	NVIC_EnableIRQ(TIM4_IRQn);


}


// IR sensor setting configuration
void IRsensor_setup(){						

	// timer 9
	if (msec > 1000 || msec < 0)
		msec = 100;

	RCC->APB1ENR |= RCC_APB2ENR_TIM9EN;		// TIM9 RCC

	TIM9->CCMR1 &= ~TIM_CCMR1_OC1M;			// Timer mode setting
	TIM9->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
	TIM9->CCMR1 |= TIM_CCMR1_OC1PE;

	TIM9->PSC = 8400 * 5 - 1;
	TIM9->ARR = (2 * msec) - 1;

	TIM9->CCER |= ~((TIM_CCER_CC1NP) | (TIM_CCER_CC1P)); // rising edge
	TIM9->CCER |= TIM_CCER_CC1P;
	TIM9->CCER |= TIM_CCER_CC1E;

	TIM9->CCR1 = (10 * msec) - 1;

	TIM9->CR1 |= TIM_CR1_CEN;

	TIM9->DIER |= TIM_DIER_CC1IE;					// CC1 Capture compare enable
	TIM9->DIER |= TIM_DIER_UIE;						// Interrupt update enable

	NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 2);            // Set the priority of TIM9 interrupt request
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
}
