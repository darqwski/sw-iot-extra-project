/*
 * Initializations.h
 *
 *  Created on: 15.12.2018
 *      Author: Darqwski
 */

#ifndef INITIALIZATIONS_H_
#define INITIALIZATIONS_H_
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

 SPI_InitTypeDef spi;
 EXTI_InitTypeDef exti;
 NVIC_InitTypeDef nvic;
 GPIO_InitTypeDef gpio;
 USART_InitTypeDef uart;

void USART1_Init(void);

void GPIOA_Init(void);
void initializeDetector(void);
void initializeSPI(void);
void initializeButtons(void);
void initializeLCDPins(void);
void send_string(const char* s);
void send_char(char c);
int __io_putchar(int c);
#endif /* INITIALIZATIONS_H_ */
