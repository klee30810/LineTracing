#include "stm32f411xe.h"

#ifndef GPIO_HEADER
#define GPIO_HEADER
//GPIO_init_t

#define GPIO_MODE_OUT 0x01
#define GPIO_MODE_IN 0x00

#define GPIO_PUPD_PU 01
#define GPIO_PUPD_PD 10
#define GPIO_PUPD_NPUPD 00

#define GPIO_OMODE_PP 0

//MODER
#define OUTPUT 0x01
#define INPUT	0x00
#define AF1 0x2
#define ANALOG 0x3

//OUTPUT TYPE
#define PUSH_PULL 0
#define OPEN_DRAIN 1

//OUTPUT SPEED
#define LOW_SPEED 00
#define MEDIUM_SPEED 01
#define FAST_SPEED 10
#define HIGH_SPEED 11

//OUTPUT PUPD
#define NO_PUPD 00
#define PULL_UP 01
#define PULL_DOWN 10
#define PUPD_REVERSED 11

//OUTPUT
#define HIGH 1
#define LOW 0

//USER PIN
#define LED_PIN	5
#define BUTTON_PIN 13

typedef struct {
GPIO_TypeDef *port;
uint32_t pin;
uint32_t mode;
uint32_t pudr;
uint32_t omode;
} GPIO_init_t;


//General config
void GPIO_AFR(GPIO_TypeDef *port, int pin, TIM_TypeDef *timer);
void GPIO_config(GPIO_TypeDef*Port, uint32_t Pin,GPIO_init_t GPIO_setting);
void GPIO_init(GPIO_TypeDef *port, int pin, int mode);
void GPIO_otype(GPIO_TypeDef *port, int pin, int type);
void GPIO_ospeed(GPIO_TypeDef *port, int pin, int speed);
void GPIO_pupd(GPIO_TypeDef *port, int pin, int pupd);
void GPIO_write (GPIO_TypeDef *port, int pin, int output);
int GPIO_read(GPIO_TypeDef *port, int pin);
void SevenSegment_init ();
void SevenSegment_on (uint8_t num);
void LED_toggle();
#endif