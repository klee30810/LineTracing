#include "myHEADER.h"
#include "myFunc.h"

PWM_t pwm_motorBA;
PWM_t pwm_motorBB;
PWM_t pwm_motorAA;
PWM_t pwm_motorAB;

PWM_t trig1, trig2;
TIM_t cap_pin1, cap_pin2, tim_int;
int mode = 0;

extern int distance1;
extern int distance2;


uint32_t UT1=0;
uint32_t UT2=0;

int sum=0;
int itr=0;
uint32_t UT1_avg = 0;
uint32_t UT2_avg = 0;

float IR1=0;
float IR2=0;
TIM_t ir_tim;

int check=0;
int flag=0;

// Bluetooth
int8_t read;

// park paramters
int park_count1 = 0;
int park_count2 = 0;
int park_count3 = 0;
int park_count4 = 0;
int park_count5 = 0;

// Follow the yellow line 
void LineTrace(int IR1, int IR2)
{
	if (IR2 < 600)
	{
		wheel(&pwm_motorBA, &pwm_motorBB, FORW, 30); // direction of two motor
		wheel(&pwm_motorAA, &pwm_motorAB, FORW, 50); // direction of two motor
	}
	else if (IR1 < 1000)   // turn left
	{
		wheel(&pwm_motorBA, &pwm_motorBB, FORW, 65); // direction of two motor
		wheel(&pwm_motorAA, &pwm_motorAB, FORW, 25); // direction of two motor
	}
	else
	{
		wheel(&pwm_motorBA, &pwm_motorBB, FORW, 42); // direction of two motor
		wheel(&pwm_motorAA, &pwm_motorAB, FORW, 45); // direction of two motor
	}

	if ((UT1_avg < 10) && (UT1_avg > 3))			// park count to move to park mode 1
	{
		park_count1++;
		printf("qweq: %d", park_count1);

	}

	if (park_count1 > 3) {			// move to the park mode 1

		mode = 1;
	}

}

// Park mode 1 to start parking
void park_mode1(uint32_t UT2_avg)
{
	printf("park_mode1");
	printf("UT1_avg = %d, UT2_avg = %d IR1 = %d, IR2 = %d\r\n", UT1_avg, UT2_avg, IR1, IR2);
	if (park_count1 > 3) {
		if (UT2_avg > 30)		// turn back left for finding parking lot
		{
			wheel(&pwm_motorBA, &pwm_motorBB, BACK, 50); // direction of two motor
			wheel(&pwm_motorAA, &pwm_motorAB, BACK, 35); // direction of two motor
			park_count2++;
		}

		else					// goes back to find the parking lot
		{
			wheel(&pwm_motorBA, &pwm_motorBB, BACK, 33); // direction of two motor 1st
			wheel(&pwm_motorAA, &pwm_motorAB, BACK, 45); // direction of two motor
		}

		if (park_count2 > 22)	// move to the park mode 2
		{
			mode = 2;
		}

	}
}

// Park mode 2 to turn back in the parking lot 
void park_mode2(uint32_t UT2_avg)
{

	if (UT1_avg < 5)		// turn back right to locate right place
	{
		wheel(&pwm_motorBA, &pwm_motorBB, BACK, 70); // direction of two motor
		wheel(&pwm_motorAA, &pwm_motorAB, BACK, 30); // direction of two motor
	}
	else					// move backward within the found parking lot according to the park_mode 1
	{
		wheel(&pwm_motorBA, &pwm_motorBB, BACK, 5); // direction of two motor
		wheel(&pwm_motorAA, &pwm_motorAB, BACK, 40); // direction of two motor
		park_count3++;
	}
	if (park_count3 > 45)	// move to the park mode 3
	{
		mode = 3;
	}

}

// Park mode 3 to finish parking
void park_mode3(uint32_t UT2_avg)
{

	if (UT1_avg < 30)
	{
		park_count4++;
	}
	if (park_count4 > 3) {							// Stop
		wheel(&pwm_motorBA, &pwm_motorBB, BACK, 0); // direction of two motor
		wheel(&pwm_motorAA, &pwm_motorAB, BACK, 0); // direction of two motor
	}

}

// GPIO setup for UT1, UT2, IR1, IR2, DCmotor1, DCmotor2
void setup()	
{
	// Ultrasonic 1
	GPIO_init(GPIOC, 7, AF);                        // trigger
	GPIO_init(GPIOB, 6, AF);                        // echo

	// Ultrasonic 2
	GPIO_init(GPIOC, 9, AF);                        // trigger
	GPIO_init(GPIOB, 10, AF);                       // echo

	// DCmotor right 
	GPIO_init(GPIOC, 8, AF);                        //DCmotor right pwm1
	GPIO_init(GPIOC, 6, AF);                        //DCmotor right pwm2

	// DCmotor left
	GPIO_init(GPIOB, 8, AF);                        //DCmotor left pwm1
	GPIO_init(GPIOB, 9, AF);                        //DCmotor left pwm2

	// IRsensor
	GPIO_init(GPIOA, 0, ANALOG);                  // 1 
	GPIO_init(GPIOA, 1, ANALOG);                  // 2

	//DCmotor setting 
	wheel_setup(&pwm_motorBA, GPIOC, 8, &pwm_motorBB, GPIOC, 6);
	wheel_setup(&pwm_motorAA, GPIOB, 8, &pwm_motorBB, GPIOB, 9);

}





int main() {
	RCC_PLL_init();
	SysTick_init();
	
	UART2_init(9600, POL);
	
	setup();
	
	IRsensor_setup(GPIOA, 0 ,GPIOA, 1, 9 ,1);	
	
	Ultrasonic_setup(&trig1, GPIOC, 7, &cap_pin1, GPIOB, 6);
	Ultrasonic_setup(&trig2, GPIOC, 9, &cap_pin2, GPIOB, 10);
	
	// Bluetooth Initialization
	UART1_init(9600);
	BT_write((uint8_t *)"Bluetooth \r\n", 18);
	
   while(1){
		 
   }
   
}

// Timer 2 ultrasensor in the left side
void TIM2_IRQHandler()
{

	Distance2(&cap_pin2);
	UT2 = (uint32_t)distance2;                     

	sum += UT2;
	itr++;

	if (itr == 40) {			// averaging 40 data
		UT2_avg = sum / 40;
		sum = 0;
		itr = 0;
	}
}


// Timer 4 ultrasensor in the front side
void TIM4_IRQHandler()
{

	// distance check
	Distance1(&cap_pin1);
	UT1 = (uint32_t)distance2;                     

	sum += UT1;
	itr++;

	if (itr == 40) {			// averaging 40 data	
		UT1_avg = sum / 40;
		sum = 0;
		itr = 0;
	}

}

// Main interrupt execution
void TIM1_BRK_TIM9_IRQHandler()
{
	if (TIM9->SR & TIM_SR_UIF)
	{
		if (mode == 0)			// Start LineTrace 
		{
			LineTrace(IR1, IR2);
			printf("UT1 = %d, UT2 = %d IR1 = %d, IR2 = %d\r\n", UT1, UT2, IR1, IR2);
		}

		else if (mode == 1)		// Park mode 1 initialization
		{
			printf("parkmode1 start\r\n");
			printf("UT1 = %d, UT2 = %d IR1 = %d, IR2 = %d\r\n", UT1, UT2, IR1, IR2);
			park_mode1(UT2);
		}

		else if (mode == 2)		// Park mode 2 initialization
		{
			printf("parkmode2 start\r\n");
			printf("UT1 = %d, UT2 = %d IR1 = %d, IR2 = %d\r\n", UT1, UT2, IR1, IR2);
			park_mode2(UT1);
		}

		else if (mode == 3)		// Park mode 3 initialization
		{
			printf("parkmode3 start\r\n");
			printf("UT1 = %d, UT2 = %d IR1 = %d, IR2 = %d\r\n", UT1, UT2, IR1, IR2);
			park_mode3(UT2);
		}

		TIM9->SR &= ~TIM_SR_UIF;
	}
}

// IR data reading from ADC
void ADC_IRQHandler()
{

	if (ADC1->SR & ADC_SR_EOC)
	{
		if (flag == 0)
		{
			IR1 = ADC_read();
			flag = 1;
		}
		else
		{
			IR2 = ADC_read();
			flag = 0;
		}
	}
}

// Bluetooth interrupt implementation
void USART1_IRQHandler()
{
	if (USART1->SR & USART_SR_RXNE)
	{
		
		read = BT_read();
		if (read == 's')					// start!
		{
			BT_write((uint8_t *)"Start!!\r\n", 9);
			timer_setup(&tim_int, TIM9, 1, 10);		// Timer 9 setting
		}

		if (read == 'f')					// Stop!
		{
			wheel(&pwm_motorBA, &pwm_motorBB, BACK,	0);		// Stop the wheel
			wheel(&pwm_motorAA, &pwm_motorBB, BACK, 0);

			timer_setup(&tim_int, TIM9, 1, 10);		// Timer 9 setting
			BT_write((uint8_t *)"Finish!!\r\n", 10);
		}

	}
}
