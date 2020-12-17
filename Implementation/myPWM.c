#include "stm32f4xx.h"
#include "myPWM.h"


// connect proper pwm pin with the timer and channels
void PWM_pinmap(PWM_t *pwm){
   if (pwm->port == GPIOA && (pwm->pin == 8)) {
      pwm->timer = TIM1;
      pwm->ch = 1;
   }
   else if (pwm->port == GPIOA && (pwm->pin == 9)) {
      pwm->timer = TIM1;
      pwm->ch = 2;
   }
   else if (pwm->port == GPIOA && (pwm->pin == 10)) {
      pwm->timer = TIM1;
      pwm->ch = 3;
   }
   else if (pwm->port == GPIOA && (pwm->pin == 11)) {
      pwm->timer = TIM1;
      pwm->ch = 4;
   }
   else if (pwm -> port == GPIOA && (pwm ->pin == 5 || pwm ->pin == 0 || pwm ->pin == 15)){   
      pwm -> timer = TIM2;
      pwm -> ch = 1;
   }
   else if   ((pwm -> port == GPIOA && pwm->pin == 1)||(pwm -> port == GPIOB && pwm->pin == 3)){
      pwm -> timer = TIM2;
      pwm -> ch = 2;
   }
   else if   (pwm -> port == GPIOB && pwm->pin == 10){
      pwm -> timer = TIM2;
      pwm -> ch = 3;
   }
   else if   ((pwm -> port == GPIOA && pwm->pin == 6)||(pwm -> port == GPIOB && pwm->pin == 4)||(pwm -> port == GPIOC && pwm->pin == 6)){
      pwm -> timer = TIM3;
      pwm -> ch = 1;
   }
   else if   ((pwm -> port == GPIOB && pwm->pin == 5)||(pwm -> port == GPIOC && pwm->pin == 7)){
      pwm -> timer = TIM3;
      pwm -> ch = 2;
   }
   else if   (pwm -> port == GPIOC && pwm->pin == 8){
      pwm -> timer = TIM3;
      pwm -> ch = 3;
   }
   else if   (pwm -> port == GPIOC && pwm->pin == 9){
      pwm -> timer = TIM3;
      pwm -> ch = 4;
   }
   else if (pwm->port == GPIOB && pwm->pin == 6) {
      pwm->timer = TIM4;
      pwm->ch = 1;
   }
   else if (pwm->port == GPIOB && pwm->pin == 7) {
      pwm->timer = TIM4;
      pwm->ch = 2;
   }
   else if (pwm->port == GPIOB && pwm->pin == 8) {
      pwm->timer = TIM4;
      pwm->ch = 3;
   }
   else if (pwm->port == GPIOB && pwm->pin == 9) {
      pwm->timer = TIM4;
      pwm->ch = 4;
   }
}

// PWM initialization setting generalization   
void PWM_init(PWM_t *pwm, GPIO_TypeDef *Port, int pin) {
      
    pwm->port = Port;
   pwm->pin = pin;
   
   PWM_pinmap(pwm);

   int pose;
   if((pwm->timer == TIM1)||(pwm->timer == TIM2)) { pose = 0; }  //Verify TIM pose
   else if(pwm->timer == TIM3) { pose = 1; }
   else if(pwm->timer == TIM4) { pose = 2; }
   else if(pwm->timer == TIM5) { pose = 3; }
   else if(pwm->timer == TIM9) { pose = 16; }
   else if(pwm->timer == TIM10) { pose = 17; }
   else if(pwm->timer == TIM11) { pose = 18; }

   if((pwm->timer == TIM2) || (pwm->timer == TIM3) || (pwm->timer == TIM4) || (pwm->timer == TIM5)) { 
   RCC->APB1ENR               |=  1UL << pose;      }                       // Enable TIMER clock
   else { RCC->APB2ENR  |=  1UL << pose; }
   
   pwm->timer->CR1           &= ~(1UL<<4);                                   // Counting direction: 0 = up-counting, 1 = down-counting
   
  pwm->timer->PSC          = 83;                                  // f_cnt = 1MHz 
                              
   pwm->timer->ARR          = 999;                                                // Auto-reload: Upcouting (0..ARR)
   

   if((pwm->ch%2) == 1) { pose = 3;}                                       //Verify channel by %
   else if((pwm->ch%2) == 0) {pose = 11;}
   
   if((pwm->ch>=1) & (pwm->ch<= 2)) {                                       // Set CCMR1 or CCMR2
   //pwm->timer->CCMR1       &= ~ (0xF << pose);                    // Clear ouput compare mode bits for channel 1
       pwm->timer->CCMR1     |= (13<<pose);                                     // Set OC1M value for PWM Mode 1 output on ch1
     pwm->timer->CCMR1     |= (6<<4);                                     

   } else if((pwm->ch>=3) & (pwm->ch<=4)) {
//   pwm->timer->CCMR2       &= ~(0xF << pose);
   pwm->timer->CCMR2     |= (13<<pose);
   }
   
   pose = 4*(pwm->ch-1);   
   
   pwm->timer->CCER       &= ~(3<<pose);                                        
   pwm->timer->CCER       |= (1<<pose);                                     // select active high Enable output for CHn          

   
   if(pwm->ch == 1) {                                                     // Set CCRx by channel
      pwm->timer->CCR1       = 500; }
   else if(pwm->ch == 2) {                              
      pwm->timer->CCR2       = 500;   }
   else if(pwm->ch == 3) {                           
      pwm->timer->CCR3       = 500; }
   else if(pwm->ch == 4) {                              
      pwm->timer->CCR4       = 500; }
   
   if(pwm->timer==TIM1) pwm->timer->BDTR |= TIM_BDTR_MOE;				// TIM1 needs BDTR setting
   
   pwm->timer->CR1      &=~TIM_CR1_CEN;                         // Counter reset      
   pwm->timer->CR1        |= TIM_CR1_CEN;                              // Enable counter
      
}
   
void PWM_period_ms(PWM_t *pwm, float period_ms) {            //set PWM period as ms
   float value = period_ms * 1000;
   
   pwm->timer->ARR   =   value - 1;
}
   
void PWM_pulsewidth_ms(PWM_t *pwm, float pulsewidth_ms){
   if(pwm->ch == 1) {                                                     // Set CCRx by channel
    pwm->timer -> CCR1 &= ~(0xFFFF);
    pwm->timer -> CCR1 |= (int)(1000*pulsewidth_ms); }
   else if(pwm->ch == 2) {                              
      pwm->timer -> CCR2 &= ~(0xFFFF);
   pwm->timer -> CCR2 |= (int)(1000*pulsewidth_ms);   }
   else if(pwm->ch == 3) {                           
      pwm->timer -> CCR3 &= ~(0xFFFF);
   pwm->timer -> CCR3 |= (int)(1000*pulsewidth_ms); }
   else if(pwm->ch == 4) {                              
      pwm->timer -> CCR4 &= ~(0xFFFF);
   pwm->timer -> CCR4 |= (int)(1000*pulsewidth_ms); }
   
   
}
   
void   PWM_duty(PWM_t *pwm, float duty) {                      //set duty ratio as %
   float value = ((pwm->timer->ARR) + 1) * duty - 1;           
   
      if(pwm->ch == 1) {                                          // chnnel setting
      pwm->timer->CCR1       = value; }
   else if(pwm->ch == 2) {                              
      pwm->timer->CCR2       = value;   }
   else if(pwm->ch == 3) {                              
      pwm->timer->CCR3       = value; }
   else if(pwm->ch == 4) {                              
      pwm->timer->CCR4       = value; }

   }
   