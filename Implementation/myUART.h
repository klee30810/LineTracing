#include "stm32f411xe.h"
#include <stdio.h>

#ifndef __MY_UART_H
#define   __MY_UART_H

#define POL 0
#define INT 1
void UART1_init(int baud_rate);
void UART1_GPIO_init(void);
void UART2_init(int baud_rate, int mode);
void UART2_GPIO_init(void);
void USART_init (USART_TypeDef * USARTx, int baud_rate, int mode);
void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_read (USART_TypeDef * USARTx);
void USART_delay(uint32_t us);
void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter);
void BT_GPIO_init();

void BT_init(uint32_t baud_rate);
uint8_t BT_read ();
void BT_write(uint8_t *buffer, uint32_t nBytes);


#endif