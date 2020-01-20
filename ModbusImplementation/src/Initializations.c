/*
 * Initializations.c
 *
 *  Created on: 15.12.2018
 *      Author: Darqwski
 */
#include "Initializations.h"
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
#include "lcd_controller.h"
void initializeUsart1(void)
{
    USART_InitTypeDef usart1_init_struct;
    GPIO_InitTypeDef gpioa_init_struct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO |
                           RCC_APB2Periph_GPIOA, ENABLE);


    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN9 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa_init_struct);

    USART_Cmd(USART1, ENABLE);
    usart1_init_struct.USART_BaudRate = 9600;
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;
    usart1_init_struct.USART_StopBits = USART_StopBits_1;
    usart1_init_struct.USART_Parity = USART_Parity_No ;
    usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
}

void initializeLCDPins(void){
	 gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; // SCK, MOSI
	 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	 gpio.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &gpio);

	 gpio.GPIO_Pin = GPIO_Pin_6; // MISO
	 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &gpio);

	 gpio.GPIO_Pin = LCD_DC|LCD_CE|LCD_RST;
	 gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOC, &gpio);
	 GPIO_SetBits(GPIOC, LCD_CE|LCD_RST);
}
void initializeButtons(void){
	 gpio.GPIO_Pin = GPIO_Pin_6;
	 gpio.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOC, &gpio);
	gpio.GPIO_Pin = GPIO_Pin_8;
	 gpio.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOC, &gpio);
}
void initializeDiods(void){
	gpio.GPIO_Pin = GPIO_Pin_4;
			gpio.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &gpio);
			 gpio.GPIO_Pin = GPIO_Pin_9;
				gpio.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOC, &gpio);
}
void initializeDetector(void){
	 gpio.GPIO_Pin = GPIO_Pin_11;
	 gpio.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOC, &gpio);
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);
	EXTI_StructInit(&exti);
	 exti.EXTI_Line = EXTI_Line11;
	 exti.EXTI_Mode = EXTI_Mode_Interrupt;
	 exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	 exti.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&exti);

	 nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	 nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	 nvic.NVIC_IRQChannelSubPriority = 0x00;
	 nvic.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&nvic);
}
void initializeSPI(void){
	 SPI_StructInit(&spi);
	 spi.SPI_Mode = SPI_Mode_Master;
	 spi.SPI_NSS = SPI_NSS_Soft;
	 spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	 SPI_Init(SPI1, &spi);
	 SPI_Cmd(SPI1, ENABLE);
}

