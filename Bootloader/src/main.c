/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
#include "help_library.h"
/*Flash unlocking*/
#define FLASH_KEY1      ((uint32_t)0x45670123)
#define FLASH_KEY2      ((uint32_t)0xCDEF89AB)
/* First page address */
#define Start_Page        		0x08002000
/* Application start address */
#define APPLICATION_ADDRESS     0x08002000
#define CONFIGURATION_ADDRES	0x08001800
/*Place for updateFirmware flag*/
const volatile uint8_t *userConfig=(const volatile uint8_t *)CONFIGURATION_ADDRES;

typedef enum {FALSE, TRUE} boolean;
typedef void (*pFunction)(void);
const int flash_page_size = 1024;
boolean checkFirmwareUpdate(){
	if(userConfig[0] == 0xFF)
		return TRUE;
	return FALSE;
}


/**
 *
 * To writing something to flash procedure
 * 1. unlock_flash(page_addr) 0x8002000,0x8002400,0x8002800,0x8002C00 ... 0x801FC00
 * 2. *(__IO uint16_t*)(page_addr+<0-1023>) = 0x0000, 16-bit value
 * 3. lock_flash()
 */
void unlock_flash(uint32_t ADDRES){
	/* Unlock flash write with special values*/
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
	/* Clear ALLL*/
	while((FLASH->SR&FLASH_SR_BSY));
	/*Setting flag*/
	FLASH->CR |= FLASH_CR_PER;
	/*Setting Address to write*/
	FLASH->AR = ADDRES;
	/*Setting flag*/
	FLASH->CR |= FLASH_CR_STRT;
	/* Clear ALLL*/
	while((FLASH->SR&FLASH_SR_BSY));
	/*Setting last flags to unlock*/
	FLASH->CR &= ~FLASH_CR_PER;
	FLASH->CR |= FLASH_CR_PG;
}
/* locking flash function */
void lock_flash(){
	/*Setting flags to lock*/
	FLASH->CR &= ~FLASH_CR_PG;
	FLASH->CR |= FLASH_CR_LOCK;
}
/* setting special value variable */
void set_new_firmware_flag(char flagVal){
			unlock_flash(CONFIGURATION_ADDRES);
			while(FLASH->SR&FLASH_SR_BSY);
			*(__IO uint16_t*)(CONFIGURATION_ADDRES) =flagVal;
			lock_flash();
}
/* setting value firmware flag as updated */
void set_firmware_updated(){
	set_new_firmware_flag(0x00);
}
/* setting value firmware flag as NOT updated */
void set_firmware_need_update(){
	set_new_firmware_flag(0xFF);
}





boolean performFirmwareUpdate(){
	/* Initialize data */
	char stringi[4];
	char read_bufor [flash_page_size];
	int read_bufor_length=0;
	int read_bufor_sum=0;
	int page_number = 0;
	boolean usart_data_received = FALSE;
	/* Initialize functions */
	set_usart2();
	set_delay();

	while(TRUE){
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0){
			set_firmware_updated();
			return FALSE;
		}
		/* Receiving data from USART */
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
			/* Noticed that program start receiving data*/
			usart_data_received=TRUE;
			/* Write data to 1024 bytes*/
			read_bufor[read_bufor_length++] = USART_ReceiveData(USART2);
			/* Giving time for connection errors*/
			timer_ms=1000;
			/* Blink diode to inform that program is working*/
			if(read_bufor_length%flash_page_size==0) {
				set_pin_on(PIN_C,PIN_8);
			}
			else if(read_bufor_length%flash_page_size==flash_page_size/2) {
				set_pin_off(PIN_C,PIN_8);
			}
		}


		/* Save one flash page, with received data*/

		if(read_bufor_length%(flash_page_size) == 0&&read_bufor_length!=0&& usart_data_received== TRUE){
			set_firmware_updated();

			unlock_flash(Start_Page+flash_page_size*page_number);

			/* loop for elements, i+2 means that two bytes is set to one byte*/

			for(int i=0; i<flash_page_size;  i+=2) {
				while((FLASH->SR&FLASH_SR_BSY));
				/* Start_page address is defined as 0x8002000 , flash_page_size is 1024*/
				*(__IO uint16_t*)(Start_Page+flash_page_size*page_number + i)=read_bufor[i] | (read_bufor[i+1]<<8);
			}
			lock_flash();

			/*Update page number*/
			page_number++;
			/*Update all sum*/
			read_bufor_sum+=flash_page_size;
			/*Reset bufor reading bufor length, to override data*/
			read_bufor_length=0;
			set_pin_on(PIN_C,PIN_8);

		}


		/* Save last page to flash after 1 second*/

		else if(usart_data_received == TRUE&&timer_ms == 0){
			set_pin_on(PIN_C,PIN_9);

			unlock_flash(Start_Page+flash_page_size*page_number);
			/* loop for elements, i+2 means that two bytes is set to one byte*/
			for(int i=page_number*flash_page_size; i<page_number*flash_page_size+read_bufor_length;  i+=2) {
				while((FLASH->SR&FLASH_SR_BSY));
				*(__IO uint16_t*)(Start_Page+ i)=read_bufor[i] | (read_bufor[i+1]<<8);
				}
			lock_flash();
			/*Send information of received data*/
			send_string("saved ");
			///itoa(read_bufor_sum+read_bufor_length,stringi,10);
			///send_string(stringi);
			send_string(" bytes \n");
			send_string("DONE!");
			set_pin_on(PIN_C,PIN_8);

			/* end updating, change flag*/
			set_firmware_updated();
			 return TRUE;

			}
		else if(usart_data_received == TRUE&&timer_ms == 0){
			set_firmware_updated();
			return TRUE;
		}

	}
	/* if something goes wrong*/
	return FALSE;

}

/* struct for jump*/
typedef void (*pFunction)(void);

/**
**===========================================================================
**  Main: Bootloader
**===========================================================================
*/

int boot_main(void)
{
	/* initializing */
	set_pin_out(PIN_C,PIN_8);
	set_pin_on(PIN_C,PIN_8);

	pFunction appEntry;

	/* check update*/
	set_pin_on(PIN_C,PIN_8);


	if(checkFirmwareUpdate())
		performFirmwareUpdate();

	/* Get the application entry point (Second entry in the application vector table) */
	set_pin_off(PIN_C,PIN_8);

	appEntry = (pFunction) *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);




	appEntry();

	while(1);

}



