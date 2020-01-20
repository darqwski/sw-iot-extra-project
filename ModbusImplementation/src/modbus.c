/*
 * modbus.c
 *
 *  Created on: 09.10.2018
 *      Author: Darqwski
 */
#include "modbus.h"
#include "help_library.h"
unsigned int read_bufor_length = 0;
unsigned int send_bufor_length = 0;


/*************************************************************************************************************************/
/**********************                 FLASH EDITING  								**************************************/
/*************************************************************************************************************************/

void unlock_flash(uint32_t ADDRES){
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = ADDRES;
	FLASH->CR |= FLASH_CR_STRT;
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR &= ~FLASH_CR_PER;
	FLASH->CR |= FLASH_CR_PG;
}
void lock_flash(){
	FLASH->CR &= ~FLASH_CR_PG;
	FLASH->CR |= FLASH_CR_LOCK;
}


/*************************************************************************************************************************/
/********************** REGISTER EDITS												**************************************/
/*************************************************************************************************************************/

unsigned short get_input_register(int address){
	return input_register[address];
}


unsigned short get_holding_register(int address){
	return holding_register[address];
}
void write_holding_register(int address, unsigned short int value){
	holding_register[address] = value;
}
void write_input_register(int address, unsigned short int value){
	holding_register[address] = value;
}
char get_discrete_input(int address){
	unsigned short index=address/8;
	unsigned short pos=address%8;
	char val = discrete_inputs[index];
	return  (val& ( 1 << pos )) >> pos;
}
char get_bit(int pos, unsigned short val){
	return  (val& ( 1 << pos )) >> pos;
}
char get_coil(int address){
	unsigned short index=address/8;
	unsigned short pos=address%8;
	char val = coils[index];
	return  (val& ( 1 << pos )) >> pos;
}
void write_coil(int address, char value){
	unsigned short index=address/8;
	unsigned short pos=address%8;
	char val = coils[index];
		if((val& ( 1 << pos )) >> pos&&value == 0)
			{
			switch(pos){
			case 0:
				val-=0b00000001;
				break;
			case 1:
				val-=0b00000010;
				break;
			case 2:
				val-=0b00000100;
				break;
			case 3:
				val-=0b00001000;
				break;
			case 4:
				val-=0b00010000;
				break;
			case 5:
				val-=0b00100000;
				break;
			case 6:
				val-=0b01000000;
				break;
			case 7:
				val-=0b10000000;
				break;

			}
		}
		else if(((val& ( 1 << pos )) >> pos == 0)&&value == 1){
			switch(pos){
					case 0:
						val+=0b00000001;
						break;
					case 1:
						val+=0b00000010;
						break;
					case 2:
						val+=0b00000100;
						break;
					case 3:
						val+=0b00001000;
						break;
					case 4:
						val+=0b00010000;
						break;
					case 5:
						val+=0b00100000;
						break;
					case 6:
						val+=0b01000000;
						break;
					case 7:
						val+=0b10000000;
						break;

						}
		}

	coils[index] = val;
}

void write_discrete_input(int address, char value){
	unsigned short index=address/8;
	unsigned short pos=address%8;
	char val = discrete_inputs[index];
		if((val& ( 1 << pos )) >> pos&&value == 0)
			{
			switch(pos){
			case 0:
				val-=0b00000001;
				break;
			case 1:
				val-=0b00000010;
				break;
			case 2:
				val-=0b00000100;
				break;
			case 3:
				val-=0b00001000;
				break;
			case 4:
				val-=0b00010000;
				break;
			case 5:
				val-=0b00100000;
				break;
			case 6:
				val-=0b01000000;
				break;
			case 7:
				val-=0b10000000;
				break;

			}
		}
		else if(((val& ( 1 << pos )) >> pos == 0)&&value == 1){
			switch(pos){
					case 0:
						val+=0b00000001;
						break;
					case 1:
						val+=0b00000010;
						break;
					case 2:
						val+=0b00000100;
						break;
					case 3:
						val+=0b00001000;
						break;
					case 4:
						val+=0b00010000;
						break;
					case 5:
						val+=0b00100000;
						break;
					case 6:
						val+=0b01000000;
						break;
					case 7:
						val+=0b10000000;
						break;

						}
		}

		discrete_inputs[index] = val;
}

/*************************************************************************************************************************/
/********************** REGISTER EDITS END												**************************************/
/*************************************************************************************************************************/


/* FOR ERRORS PART*/
//enum modbus_function_status {OK = 0x00,ExceptionCode_1=0x01, ExceptionCode_2=0x02, ExceptionCode_3=0x03,ExceptionCode_4=0x04 };

/************************************************************************************************************************/
/**********************MODBUS FUNCTIONS BEGIN										**************************************/
/************************************************************************************************************************/

enum modbus_function_status fc_read_coils(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2){
	unsigned int address = start_address_1*(16*16)+start_address_2;
	unsigned int quantity = (quantity_register_1*(16*16)+quantity_register_2);
	if(!(0x0001<=quantity&&quantity<=0x07D0))
		return ExceptionCode_2;
	if(!(0x0000<=address&&address<=0xFFFF&&0x0001<=address+quantity&&address+quantity<=0xFFFF))
		return ExceptionCode_3;
	char bytes_count = quantity/8;
	if(quantity%8!=0)bytes_count++;
	send_bufor[send_bufor_length++] = bytes_count;

	char temp_coil = 0b00000000;
	int coil_counter = 0;
	for(unsigned int i = address;i<address+quantity;i++,coil_counter++){
		if(coil_counter == 8){
			send_bufor[send_bufor_length++]	 = temp_coil;
			coil_counter=0;
			temp_coil = 0b00000000;

		}

		int tempCoilVal = get_coil(i);
		if(tempCoilVal == 0x00)continue;
		switch(coil_counter){
			case 0:
				temp_coil+=0b00000001;
				break;
			case 1:
				temp_coil+=0b00000010;
				break;
			case 2:
				temp_coil+=0b00000100;
				break;
			case 3:
				temp_coil+=0b00001000;
				break;
			case 4:
				temp_coil+=0b00010000;
				break;
			case 5:
				temp_coil+=0b00100000;
				break;
			case 6:
				temp_coil+=0b01000000;
				break;
			case 7:
				temp_coil+=0b10000000;
				break;
			}

	 }
	if(coil_counter!=0){
		send_bufor[send_bufor_length++]	 = temp_coil;
		temp_coil = 0b00000000;
	}
	return Function_OK;
}

enum modbus_function_status fc_read_discrete_inputs(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2){
	unsigned int address = start_address_1*(16*16)+start_address_2;
	unsigned int quantity = (quantity_register_1*(16*16)+quantity_register_2);

	if(!(0x0001<=quantity&&quantity<=0x07D))
		return ExceptionCode_2;
	if(!(0x0000<=address&&address<=0xFFFF&&0x0001<=address+quantity&&address+quantity<=0xFFFF))
		return ExceptionCode_3;

	char bytes_count = quantity/8;

	if(quantity%8!=0)bytes_count++;
	send_bufor[send_bufor_length++] = bytes_count;

	char temp_coil = 0b00000000;
	int coil_counter = 0;
	for(unsigned int i = address;i<address+quantity;i++,coil_counter++){
		if(coil_counter == 8){
			send_bufor[send_bufor_length++]	 = temp_coil;
			coil_counter=0;
			temp_coil = 0b00000000;

		}

		int tempCoilVal = get_discrete_input(i);
		if(tempCoilVal == 0x00)continue;
		switch(coil_counter){
			case 0:
				temp_coil+=0b00000001;
				break;
			case 1:
				temp_coil+=0b00000010;
				break;
			case 2:
				temp_coil+=0b00000100;
					break;
			case 3:
				temp_coil+=0b00001000;
				break;
			case 4:
				temp_coil+=0b00010000;
				break;
			case 5:
				temp_coil+=0b00100000;
				break;
			case 6:
				temp_coil+=0b01000000;
				break;
			case 7:
				temp_coil+=0b10000000;
				break;
			}

	 }
	if(coil_counter!=0){
		send_bufor[send_bufor_length++]	 = temp_coil;
		temp_coil = 0b00000000;
	}
	return Function_OK;

}


enum modbus_function_status fc_read_holding_registers(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2){
	unsigned int address = start_address_1*(16*16)+start_address_2;
	unsigned int quantity = quantity_register_1*(16*16)+quantity_register_2;

	if(!(0x0001<=quantity&&quantity<=0x07D))
			return ExceptionCode_2;
		if(!(0x0000<=address&&address<=0xFFFF&&0x0001<=address+quantity&&address+quantity<=0xFFFF))
			return ExceptionCode_3;

	send_bufor[send_bufor_length++] = quantity;


	for(unsigned int i = address;i<address+quantity;i++){
		send_bufor[send_bufor_length++] = holding_register[i] >> 8;
		send_bufor[send_bufor_length++] = holding_register[i] & 0xFF;

	 }
	return Function_OK;

}

enum modbus_function_status fc_read_input_registers(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2){
	unsigned int address = start_address_1*(16*16)+start_address_2;
	unsigned int quantity = quantity_register_1*(16*16)+quantity_register_2;
	if(!(0x0001<=quantity&&quantity<=0x07D0))
		return ExceptionCode_2;
	if(!(0x0000<=address&&address<=0xFFFF&&0x0001<=address+quantity&&address+quantity<=0xFFFF))
		return ExceptionCode_3;

	send_bufor[send_bufor_length++] = quantity;


	for(unsigned int i = address;i<address+quantity;i++){
		send_bufor[send_bufor_length++] = input_register[i] >> 8;
		send_bufor[send_bufor_length++] = input_register[i] & 0xFF;

	 }
	return Function_OK;

}

enum modbus_function_status fc_write_single_register(char start_address_1,char start_address_2,char value_register_1,char value_register_2){
	unsigned int address = start_address_1*(16*16)+start_address_2;
	unsigned int value = value_register_1*(16*16)+value_register_2;

	if(!(value>=0x00&&value<=0xFFFF))
		return ExceptionCode_2;
	if(!(0x0000<=address&&address<=0xFFFF))
		return ExceptionCode_3;

	holding_register[address] = value;
	send_bufor[send_bufor_length++] = address >> 8;
	send_bufor[send_bufor_length++] = address & 0xFF;
	send_bufor[send_bufor_length++] = holding_register[address] >> 8;
	send_bufor[send_bufor_length++] = holding_register[address] & 0xFF;

	return Function_OK;

}
enum modbus_function_status fc_write_single_coil(char start_address_1,char start_address_2,char value_register_1,char value_register_2){
	unsigned int address = start_address_1*(16*16)+start_address_2;
	unsigned int value = value_register_1*(16*16)+value_register_2;

	if(!(value==0x00&&value==0xFF00))
		return ExceptionCode_2;
	if(!(0x0000<=address&&address<=0xFFFF))
		return ExceptionCode_3;

	if(value == 0x0000)
		write_coil(address,0);
	else if(value == 0xFF00)
		write_coil(address,1);

	send_bufor[send_bufor_length++] = address >> 8;
	send_bufor[send_bufor_length++] = address & 0xFF;
	send_bufor[send_bufor_length++] =value >> 8;
	send_bufor[send_bufor_length++] = value & 0xFF;

	return Function_OK;

}
enum modbus_function_status fc_write_multiple_register(char read_bufor[]){
	 short address = read_bufor[2]*(16*16)+ read_bufor[3];
	 short quantity_of_registers = read_bufor[4]*(16*16)+ read_bufor[5];
	 short byte_count = read_bufor[6];

	 if(!(quantity_of_registers>=0x01&&quantity_of_registers<=0x007B&&quantity_of_registers*2==byte_count))
		 		return ExceptionCode_2;
		 	if(!(0x0000<=address&&address<=0xFFFF&&0x0001<=address+quantity_of_registers&&address+quantity_of_registers<=0xFFFF))
		 		return ExceptionCode_3;

	for(int i = 7;i<byte_count+7;i+=2){
		holding_register[address+(i-7)/2] =  read_bufor[i]*(16*16) + read_bufor[i+1];

	}
	send_bufor[send_bufor_length++] = address>> 8;
	send_bufor[send_bufor_length++] = address & 0xFF;

	send_bufor[send_bufor_length++] =quantity_of_registers>> 8;
	send_bufor[send_bufor_length++] = quantity_of_registers & 0xFF;

	return Function_OK;

}
enum modbus_function_status fc_write_multiple_coil(char read_bufor[]){
	 short address = read_bufor[2]*(16*16)+ read_bufor[3];
	 short quantity_of_coils = read_bufor[4]*(16*16)+ read_bufor[5];
	 short byte_count = read_bufor[6];
	 short checked_quantity_of_coils = quantity_of_coils/8;
	 if(quantity_of_coils%8!=0)checked_quantity_of_coils++;

	 if(!(quantity_of_coils>=0x01&&quantity_of_coils<=0x07B0&&checked_quantity_of_coils==byte_count))
	 		return ExceptionCode_2;
	 	if(!(0x0000<=address&&address<=0xFFFF&&0x0001<=address+quantity_of_coils&&address+quantity_of_coils<=0xFFFF))
	 		return ExceptionCode_3;

	 for(int i = 7;i<byte_count+7;i++){
		 char tempCoils = read_bufor[i];
		 for(int j = 0;j<8;j++){
			if(get_bit(j,tempCoils) == 0x00)
				write_coil(address+((i-7)*8)+j , 0x00);
			else
				write_coil(address+((i-7)*8)+j , 0x01);

		 }
	}
	send_bufor[send_bufor_length++] = address>> 8;
	send_bufor[send_bufor_length++] = address & 0xFF;

	send_bufor[send_bufor_length++] =quantity_of_coils>> 8;
	send_bufor[send_bufor_length++] = quantity_of_coils & 0xFF;

	return Function_OK;

}
enum modbus_function_status fc_update_firmaware(void){
						unlock_flash(CONFIGURATION_ADDRES);
						while(FLASH->SR&FLASH_SR_BSY);
						 *(__IO uint16_t*)(CONFIGURATION_ADDRES) = 0xFF;
						lock_flash();
						NVIC_SystemReset();
						return Function_OK;
}
/************************************************************************************************************************/
/**********************MODBUS FUNCTIONS END											**************************************/
/************************************************************************************************************************/



int check_CRC_sum(void){
 unsigned short int sum =	CRC16(read_bufor,read_bufor_length-2 );
 unsigned short int received_sum = read_bufor[read_bufor_length-1]*(16*16)+ read_bufor[read_bufor_length-2];
 if(sum == received_sum )
	 return 1;
 return 0;
}

void read_rpi_request(void){
	switch(read_bufor[0]){
		case 0x00: // Zaswiec wszystko
			write_coil(0,0x01);
			write_coil(1,0x01);
			write_coil(2,0x01);
			write_coil(3,0x01);
			send_bufor_length=0;
			send_bufor[send_bufor_length++] =0x5F;
			send_bufor[send_bufor_length++] =0x5F;
		break;
		case 0x01: // Wylacz Piec
			write_holding_register(1, 0);
			send_bufor_length=0;
			send_bufor[send_bufor_length++] =0x5F;
			send_bufor[send_bufor_length++] =0x5F;
		break;
		case 0x02: // Wlacz Piec
			write_holding_register(1, 1);
			send_bufor_length=0;
			send_bufor[send_bufor_length++] =0x5F;
			send_bufor[send_bufor_length++] =0x5F;
		break;
		case 0x03: // Rozpal Piec
				write_holding_register(1, 2);
			break;
		case 0x04: // Doloz
			write_holding_register(1, 3);
			write_holding_register(2, read_bufor[1]);
		break;
		case 0x05: // Error
			write_holding_register(1, 4);
		break;
		case 0x0B: // Data
			fc_read_input_registers(0,0,0,4);
			send_bufor_length=0;
			for(int i=0;i<4;i++){
				send_bufor[send_bufor_length++] = input_register[i] >> 8;
				send_bufor[send_bufor_length++] = input_register[i] & 0xFF;
			}
		break;

	}
	read_bufor_length=0;

}
void read_modbus_request(void){

	if(read_bufor[0] == get_device_address()){
		send_bufor[send_bufor_length++] = read_bufor[0];
		send_bufor[send_bufor_length++] = read_bufor[1];
	}

				else {
					read_rpi_request();
					return;
				}

	char function_code = read_bufor[1];

	char error=0;
	if(check_CRC_sum() == 0)
		return;
	switch(function_code){
	case 0x00:
		break;
	case FC_READ_COILS:
		error=fc_read_coils(read_bufor[2],read_bufor[3],read_bufor[4],read_bufor[5]);
		break;
	case FC_READ_DISCRETE_INPUTS:
		error=fc_read_discrete_inputs(read_bufor[2],read_bufor[3],read_bufor[4],read_bufor[5]);

		break;
	case FC_READ_HOLDING_REGISTERS:
		error=fc_read_holding_registers(read_bufor[2],read_bufor[3],read_bufor[4],read_bufor[5]);
		break;
	case FC_READ_INPUT_REGISTERS:
		error=fc_read_input_registers(read_bufor[2],read_bufor[3],read_bufor[4],read_bufor[5]);
		break;
	case FC_WRITE_SINGLE_COIL:
		error=fc_write_single_coil(read_bufor[2],read_bufor[3],read_bufor[4],read_bufor[5]);
		break;
	case FC_WRITE_SINGLE_REGISTER:
		error=fc_write_single_register(read_bufor[2],read_bufor[3],read_bufor[4],read_bufor[5]);
		break;
	case 0x07:
		break;
	case 0x08:
		break;
	case 0x09:
		break;
	case FC_WRITE_MULTIPLE_REGISTERS:
		error=fc_write_multiple_register(read_bufor);
		break;
	case FC_WRITE_MULTIPLE_COILS:
		error=fc_write_multiple_coil(read_bufor);
		break;
	case FC_UPDATE_FIRMWARE:
		error=fc_update_firmaware();
		break;
	default:
		error= ExceptionCode_1;
		break;
	}


	if(error!=Function_OK){
				send_bufor_length = 0;
				send_bufor[send_bufor_length++] = get_device_address();
				send_bufor[send_bufor_length++] = function_code + MODBUS_ERROR_FUNCTIONCODE_JUMP;
				send_bufor[send_bufor_length++] = error;
			}
	unsigned short crc = CRC16 (send_bufor, send_bufor_length);
	send_bufor[send_bufor_length++] = crc & 0xFF;
	send_bufor[send_bufor_length++] = crc >> 8;


	read_bufor_length=0;

}
void read_data_module(void){


}
int get_device_address(void){
return DEVICE_ID;
}
void send_bufor_response(void){
for(int i = 0 ;i<send_bufor_length;i++)
	send_char(send_bufor[i]);

send_bufor_length = 0;
}
unsigned short int CRC16 (const char *nData, unsigned short int wLength)
{
static const unsigned short int wCRCTable[] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

char nTemp;
unsigned short int wCRCWord = 0xFFFF;

   while (wLength--)
   {
      nTemp = *nData++ ^ wCRCWord;
      wCRCWord >>= 8;
      wCRCWord ^= wCRCTable[nTemp];
   }
   return wCRCWord;

}
