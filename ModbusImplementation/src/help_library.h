/*
 * help_library.h
 *
 *  Created on: 11.10.2018
 *      Author: Darqwski
 */

#ifndef HELP_LIBRARY_H_
#define HELP_LIBRARY_H_

#define PIN_A		0
#define PIN_B		1
#define PIN_C		2
#define PIN_D		3

#define PIN_0		0x0001
#define PIN_1		0x0002
#define PIN_2		0x0004
#define PIN_3		0x0008
#define PIN_4		0x0010
#define PIN_5		0x0020
#define PIN_6		0x0040
#define PIN_7		0x0080
#define PIN_8		0x0100
#define PIN_9		0x0200
#define PIN_10		0x0400
#define PIN_11		0x0800
#define PIN_12		0x1000
#define PIN_13		0x2000
#define PIN_14		0x4000
#define PIN_15		0x8000
#define APPLICATION_ADDRESS     0x08002000

#include "stm32f1xx_nucleo.h"
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	USART_InitTypeDef uart;
	TIM_TimeBaseInitTypeDef tim;
volatile uint32_t timer_ms;

void SysTick_Handler();
void set_delay();
void delay(int time);
void set_after_bootloader(void);
void set_pin_out(int pin_letter,uint16_t pin_number);
void set_pin_in(int pin_letter,uint16_t pin_number);
void set_pin_on(int pin_letter,uint16_t pin_number);
void set_pin_off(int pin_letter,uint16_t pin_number);

/*TRZEBA PRZED NIM USTAWIĆ JAKIŚ PIN*/
void set_usart2();
void set_usart1();
void set_usart2_exti();
void set_ADC1();
void set_ADC2();
int adc_read();
void set_usart2_baudrate();
void send_char(char c);
void send_string(const char* s);
#endif /* HELP_LIBRARY_H_ */
