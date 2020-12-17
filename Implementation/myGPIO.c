#include "myGPIO.h"

// GPIO AFR register setup generalization
void GPIO_AFR(GPIO_TypeDef *port, int pin, TIM_TypeDef *timer){
    int AF_num=0;
   
//select AF  
    if (timer == TIM1 || timer ==TIM2){
         AF_num = 1;     }
    if (timer == TIM3 || timer ==TIM4 ||timer ==TIM5){
         AF_num = 2;    }
      if (timer == TIM9 || timer ==TIM10 ||timer ==TIM11){
         AF_num = 3;    }
 //set AFR 
     port->AFR[pin/8]   &= ~(15   << ((pin%8)*4)); 
     port->AFR[pin/8]   |= AF_num << ((pin%8)*4);
}

// GPIO setting configuration
void GPIO_init(GPIO_TypeDef *port, int pin, int mode)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	port->MODER &= ~(3UL<<(2*pin));
	port->MODER |= mode<<(2*pin);
	
	port->OSPEEDR &= ~(3UL<<(2*pin));
	port->OSPEEDR |=   3UL<<(2*pin);
	
	port->OTYPER &= ~(1UL<<pin);
	port->PUPDR  &= ~(3UL<<(2*pin));
	
	port->AFR[0] 	|=(mode<<4*pin);
	
}

// GPIO pin mode selection
void GPIO_mode(GPIO_TypeDef *port, int pin, int mode)
{
	port->MODER &= ~(3UL<<(2*pin));
	port->MODER |=  mode<<(2*pin);
	
}

// GPIO pin output type selection
void GPIO_otype(GPIO_TypeDef *port, int pin, int type)
{
	port->OTYPER |= (type<<pin);
}

// GPIO pin output speed selection
void GPIO_ospeed(GPIO_TypeDef *port, int pin, int speed)
{
	port->OSPEEDR &= ~(3UL<<(2*pin));
	port->OSPEEDR |= speed<<(2*pin);
}

// GPIO pin pullup-pulldown selection
void GPIO_pupd(GPIO_TypeDef *port, int pin, int pupd)
{
	(port->PUPDR) &= ~(3UL<<(2*pin));
	(port->PUPDR) |= (pupd<<(2*pin));
}

// GPIO pin writting
void GPIO_write (GPIO_TypeDef *port, int pin, int output)
{
	(port->ODR) &= ~(1UL<<pin);
	(port->ODR) |= (output<<pin);
}

// GPIO pin read
int GPIO_read(GPIO_TypeDef *port, int pin)
{
	int READ;
	READ = (port->IDR) & (1UL<<pin);
	return READ;
}

// Sevensegment setup
void SevenSegment_init ()
{
	 GPIO_init(GPIOA, 5, OUTPUT); // a
   GPIO_init(GPIOA, 6, OUTPUT); // b
   GPIO_init(GPIOA, 7, OUTPUT); // c
   GPIO_init(GPIOB, 6, OUTPUT); // d
   GPIO_init(GPIOC, 7, OUTPUT); // e
   GPIO_init(GPIOA, 9, OUTPUT); // f
   GPIO_init(GPIOA, 8, OUTPUT); // g
   GPIO_init(GPIOB, 10, OUTPUT); // dp   
}

// Sevensegment turning on
void SevenSegment_on (uint8_t num)
{
	int ON = 0b00000011;
		

   if (num == 0)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 0); //D
      GPIO_write(GPIOC, 7, 0); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 1); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 1)
   {
      GPIO_write(GPIOA, 5, 1); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 1); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 1); //F
      GPIO_write(GPIOA, 8, 1); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 2)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 1); //C
      GPIO_write(GPIOB, 6, 0); //D
      GPIO_write(GPIOC, 7, 0); //E
      GPIO_write(GPIOA, 9, 1); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 3)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 0); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 1); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 4)
   {
      GPIO_write(GPIOA, 5, 1); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 1); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 5)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 1); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 0); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 6)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 1); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 0); //D
      GPIO_write(GPIOC, 7, 0); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 7)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 1); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 1); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 8)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 0); //D
      GPIO_write(GPIOC, 7, 0); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else if (num == 9)
   {
      GPIO_write(GPIOA, 5, 0); //A
      GPIO_write(GPIOA, 6, 0); //B
      GPIO_write(GPIOA, 7, 0); //C
      GPIO_write(GPIOB, 6, 1); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 0); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
   else{
     GPIO_write(GPIOA, 5, 1); //A
      GPIO_write(GPIOA, 6, 1); //B
      GPIO_write(GPIOA, 7, 1); //C
      GPIO_write(GPIOB, 6, 1); //D
      GPIO_write(GPIOC, 7, 1); //E
      GPIO_write(GPIOA, 9, 1); //F
      GPIO_write(GPIOA, 8, 0); //G
      GPIO_write(GPIOB, 10, 1);//DP
   }
		 
}

// LED_toggle
void LED_toggle()
{
GPIOA -> ODR ^= GPIO_ODR_OD5;
}