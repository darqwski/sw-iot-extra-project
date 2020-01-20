/*
 * bootFunctions.h
 *
 *  Created on: 31.10.2018
 *      Author: generalautomatic
 */

#ifndef BOOTFUNCTIONS_H_
#define BOOTFUNCTIONS_H_
#define PORTBASE 	0x20004A00 /* Counter/Timer Base */
#define PORT_FLASH 	0x0801FC00
#define PortLoad ((volatile unsigned int *) PORTBASE)  /* 32 bits */
#define PortValue ((volatile unsigned short *)(PORTBASE + 0x04)) /* 16 bits */
#define PortClear ((volatile unsigned char *)(PORTBASE + 0x08)) /* 8 bits */

#define FlashLoad (( unsigned char *) PORT_FLASH)  /* 32 bits */
#define FlashValue (( unsigned short *)(PORT_FLASH + 0x04)) /* 16 bits */
#define FlashClear (( unsigned char *)(PORT_FLASH + 0x08)) /* 8 bits */

void test(void)

{
	*PortLoad = (unsigned int) 0xFFFFFFFF;
//asm(nop);

}
void test2(void){
}
unsigned int read2(void){
	return *FlashLoad;
}
//0x20005000

void init_regs(void)
{
unsigned int int_val;
unsigned short short_val;
unsigned char char_val;
*PortLoad = (unsigned int) 0xF00FF00F;
int_val = *PortLoad;
*PortValue = (unsigned short) 0x0000;
short_val = *PortValue;
*PortClear = (unsigned char) 0x1F;
char_val = *PortClear;
}

#endif /* BOOTFUNCTIONS_H_ */
