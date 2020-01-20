/*
 * help_library.c
 *
 *  Created on: 11.10.2018
 *      Author: Darqwski
 */


#include "help_library.h"
volatile uint32_t timer_ms = 0; //deklaracja zmiennej 32 bitowej bez znaku
void SysTick_Handler()
{
    if (timer_ms) {
        timer_ms--;
    }
}
void TIM2_IRQHandler()
{
 if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
 {
	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	 if(timer_ms>0){
		 timer_ms--;
	 }

 }

 if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
 {
 TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
 }
 if (TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)
 {
 TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
 }
 if (TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET)
 {
 TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
 }
 if (TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET)
 {
 TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);

 }
}
void set_after_bootloader(void){
	uint32_t appStack;
	appStack = (uint32_t) *((__IO uint32_t*)APPLICATION_ADDRESS);
	/* Reconfigure vector table offset register to match the application location */
	//set_pin_off(PIN_C,PIN_8);
	SCB->VTOR = APPLICATION_ADDRESS;
	/* Set the application stack pointer */
	__set_MSP(appStack);
}
void set_pin_out(int pin_letter,uint16_t pin_number){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_StructInit(&gpio);

		gpio.GPIO_Pin = pin_number;
		gpio.GPIO_Mode = GPIO_Mode_Out_PP;
		switch(pin_letter){
		case PIN_A:
			GPIO_Init(GPIOA, &gpio);
			break;
		case PIN_B:
			GPIO_Init(GPIOB, &gpio);
			break;
		case PIN_C:
			GPIO_Init(GPIOC, &gpio);
			break;
		case PIN_D:
			GPIO_Init(GPIOD, &gpio);
			break;

		}
}
void set_pin_on(int pin_letter,uint16_t pin_number){
	switch(pin_letter){
			case PIN_A:
				GPIO_SetBits(GPIOA, pin_number);
				break;
			case PIN_B:
				GPIO_SetBits(GPIOB, pin_number);
				break;
			case PIN_C:
				GPIO_SetBits(GPIOC, pin_number);
				break;
			case PIN_D:
				GPIO_SetBits(GPIOD, pin_number);
				break;

			}

}
void set_pin_off(int pin_letter,uint16_t pin_number){
	switch(pin_letter){
			case PIN_A:
				GPIO_ResetBits(GPIOA, pin_number);
				break;
			case PIN_B:
				GPIO_ResetBits(GPIOB, pin_number);
				break;
			case PIN_C:
				GPIO_ResetBits(GPIOC, pin_number);
				break;
			case PIN_D:
				GPIO_ResetBits(GPIOD, pin_number);
				break;

			}

}
void set_pin_in(int pin_letter,uint16_t pin_number){



		GPIO_StructInit(&gpio);
		gpio.GPIO_Pin = pin_number;
		gpio.GPIO_Mode = GPIO_Mode_IPU;
		switch(pin_letter){
		case PIN_A:
			GPIO_Init(GPIOA, &gpio);
			break;
		case PIN_B:
			GPIO_Init(GPIOB, &gpio);
			break;
		case PIN_C:
			GPIO_Init(GPIOC, &gpio);
			break;
		case PIN_D:
			GPIO_Init(GPIOD, &gpio);
			break;

		}
}

void set_delay(){
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 SysTick_Config(SystemCoreClock / 1000);

}
void set_usart2_exti(){

		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

			gpio.GPIO_Pin = GPIO_Pin_2;
			 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
			 GPIO_Init(GPIOA, &gpio);

			 gpio.GPIO_Pin = GPIO_Pin_3;
			 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			 GPIO_Init(GPIOA, &gpio);

			 exti.EXTI_Line = EXTI_Line3;
			 exti.EXTI_Mode = EXTI_Mode_Interrupt;
			 exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			 exti.EXTI_LineCmd = ENABLE;
			 EXTI_Init(&exti);

			 nvic.NVIC_IRQChannel = EXTI3_IRQn;
			 nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
			 nvic.NVIC_IRQChannelSubPriority = 0x00;
			 nvic.NVIC_IRQChannelCmd = ENABLE;
			 NVIC_Init(&nvic);

			 USART_StructInit(&uart);
			 uart.USART_BaudRate = 9600;
			 USART_Init(USART2, &uart);
			 USART_Cmd(USART2, ENABLE);

}
void set_usart2(){

		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

		 GPIO_StructInit(&gpio);
			 gpio.GPIO_Pin = GPIO_Pin_2;
			 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
			 GPIO_Init(GPIOA, &gpio);

			 gpio.GPIO_Pin = GPIO_Pin_3;
			 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			 GPIO_Init(GPIOA, &gpio);

			 USART_StructInit(&uart);
			 uart.USART_BaudRate = 9600;
			 USART_Init(USART2, &uart);

			 USART_Cmd(USART2, ENABLE);

}
void set_usart1(){

		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		 RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);

		 GPIO_StructInit(&gpio);
			 gpio.GPIO_Pin = GPIO_Pin_9;
			 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
			 GPIO_Init(GPIOA, &gpio);

			 gpio.GPIO_Pin = GPIO_Pin_10;
			 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			 GPIO_Init(GPIOA, &gpio);

			 USART_StructInit(&uart);
			 uart.USART_BaudRate = 9600;
			 USART_Init(USART1, &uart);

			 USART_Cmd(USART1, ENABLE);

}
void set_usart2_baudrate(int rate){
	//115200
			uart.USART_BaudRate = rate;
				 USART_Init(USART2, &uart);
				 USART_Cmd(USART2, ENABLE);
}
void set_ADC1(){
	ADC_InitTypeDef adc;
		 RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

		 gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
		 gpio.GPIO_Mode = GPIO_Mode_AIN;
		 GPIO_Init(GPIOA, &gpio);

		 ADC_StructInit(&adc);
		 adc.ADC_ContinuousConvMode = DISABLE;
		 adc.ADC_NbrOfChannel = 1;
		 adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		 ADC_Init(ADC1, &adc);
		 ADC_Cmd(ADC1, ENABLE);


		 ADC_ResetCalibration(ADC1);
		 while (ADC_GetResetCalibrationStatus(ADC1));

		 ADC_StartCalibration(ADC1);
		 while (ADC_GetCalibrationStatus(ADC1));

		 ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

int adc_read(int channel)
{
 ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_13Cycles5);
 ADC_SoftwareStartConvCmd(ADC1, ENABLE);

 while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);

 return ADC_GetConversionValue(ADC1);
}

void set_ADC2(){
	ADC_InitTypeDef adc;
		 RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

		 gpio.GPIO_Pin =  GPIO_Pin_0;
		 gpio.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &gpio);



		 ADC_StructInit(&adc);
		 adc.ADC_ContinuousConvMode = ENABLE;
		 adc.ADC_NbrOfChannel = 1;
		 adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		 ADC_Init(ADC1, &adc);

		 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5);
		 ADC_Cmd(ADC1, ENABLE);

		 ADC_ResetCalibration(ADC1);
		 while (ADC_GetResetCalibrationStatus(ADC1));

		 ADC_StartCalibration(ADC1);
		 while (ADC_GetCalibrationStatus(ADC1));

		 ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void delay(int time)
{
    timer_ms = time;
    while(timer_ms > 0){};
}
void send_string(const char* s)
{
    while (*s)
        send_char(*s++);
}
void send_char(char c)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, c);
}

int __io_putchar(int c)
{
 if (c=='\n')
 send_char('\r');
 send_char(c);
 return c;
}

