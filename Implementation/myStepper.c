#include <stdint.h>
#include "myStepper.h"
#include "myRCC.h"
#include "myGPIO.h"
#include "mySystick.h"


uint32_t direction;
uint32_t step_delay;
uint32_t step_per_rev;
uint32_t motor_type;

void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4){
	
	//RCC_PLL_init();		//84MHz
	
	// Initialize variables
	direction = 0; step_delay=0; step_per_rev=0; motor_type=0;
	
	// 1st pin setting for step moter
	GPIO_init(port1,pin1,OUTPUT);
	GPIO_otype(port1,pin1,PP);
	GPIO_pupd(port1,pin1, 0);
	
	// 2nd pin setting for step moter
	GPIO_init(port2,pin2,OUTPUT);
	GPIO_otype(port2,pin2,PP);
	GPIO_pupd(port2,pin2, 0);
	
	// 3rd pin setting for step moter
	GPIO_init(port3,pin3,OUTPUT);
	GPIO_otype(port3,pin3,PP);
	GPIO_pupd(port3,pin3,0);
	
	// 4th pin setting for step moter
	GPIO_init(port4, pin4, OUTPUT);
	GPIO_otype(port4, pin4,PP);
	GPIO_pupd(port4, pin4, 0);
	
}
	
	
void Stepper_setSpeed(uint32_t speed){
	step_delay = (float)((float)(60*1000 / step_per_rev / speed) + 0.5);		// step delay is set from the given speed in milli sec
}
	
void Stepper_step (int steps_to_move){
	
	// Determine direction based on whether step is + or -
	if (steps_to_move > 0) direction = 1;						// right
	else if  (steps_to_move < 0) direction = 0;			// left
	
	// decrement the number of steps, moving one step each time:
	int i = 0;
	int step_number = steps_to_move;
	for(i = 0; i<steps_to_move; i++)
	{
		delay(step_delay);
		if (direction == 1){							// right rotation
			step_number++;
		}
		else  {														// left rotation
			step_number--;
		}
		Stepper_stepMotor(step_number % 4);
	}
}

void Stepper_stepMotor (uint32_t sequence){
	switch(sequence){
		case 0:		//1001
			GPIO_write(GPIOB, 10, OUTPUT);
			GPIO_write(GPIOB, 4, 0);
			GPIO_write(GPIOB, 5, 0);
			GPIO_write(GPIOB, 3, OUTPUT);
			break;
		
		case 1:		//1010
			GPIO_write(GPIOB, 10, OUTPUT);
			GPIO_write(GPIOB, 4, 0);
			GPIO_write(GPIOB, 5, OUTPUT);
			GPIO_write(GPIOB, 3, 0);
			break;
		
		case 2:		// 0110
			GPIO_write(GPIOB, 10, 0);
			GPIO_write(GPIOB, 4, OUTPUT);
			GPIO_write(GPIOB, 5, OUTPUT);
			GPIO_write(GPIOB, 3, 0);
			break;
		
		case 3:		// 0101
			GPIO_write(GPIOB, 10, 0);
			GPIO_write(GPIOB, 4, OUTPUT);
			GPIO_write(GPIOB, 5, 0);
			GPIO_write(GPIOB, 3, OUTPUT);
			break;
	}
}
