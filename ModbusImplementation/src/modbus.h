/*
 * modbus.h
 *
 *  Created on: 09.10.2018
 *      Author: Darqwski
 */

#ifndef MODBUS_H_
#define MODBUS_H_
#define MODBUS_ERROR_FUNCTIONCODE_JUMP			0x80
#define FC_READ_COILS 							0x01
#define FC_READ_DISCRETE_INPUTS 				0x02
#define FC_READ_HOLDING_REGISTERS 				0x03
#define FC_READ_INPUT_REGISTERS					0x04

#define FC_WRITE_SINGLE_COIL 					0x05
#define FC_WRITE_SINGLE_REGISTER 				0x06
#define FC_WRITE_MULTIPLE_COILS					0x0F
#define FC_WRITE_MULTIPLE_REGISTERS				0x10

#define FC_UPDATE_FIRMWARE						0x41


#define DEVICE_ID								0x0A
#define CONFIGURATION_ADDRES	0x08001800

unsigned short input_register[256]; 	//READ_ONLY
unsigned short holding_register[256];
unsigned short discrete_inputs[16]; 	//READ_ONLY
unsigned short coils[16];
/* FOR ERRORS PART*/
enum modbus_function_status {Function_OK = 0x00,ExceptionCode_1=0x01, ExceptionCode_2=0x02, ExceptionCode_3=0x03,ExceptionCode_4=0x04 };

/*Register edition*/
unsigned short get_input_register(int address);
unsigned short get_holding_register(int address);
char get_discrete_input(int address);
char get_coil(int address);

void write_input_register(int address, unsigned short int value);
void write_holding_register(int address, unsigned short int value);
void write_coil(int address, char value);
void write_discrete_input(int address, char value);
/*Modbus data types*/
/*Modbus functions switch*/
void send_bufor_response(void);
void read_modbus_request(void);
void read_data_module();
char read_bufor[256];
char send_bufor[256];
unsigned int read_bufor_length;
unsigned int send_bufor_length;

/*modbus functions*/
enum modbus_function_status fc_read_discrete_inputs(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2);
enum modbus_function_status fc_read_coils(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2);
enum modbus_function_status fc_read_holding_registers(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2);
enum modbus_function_status fc_read_input_registers(char start_address_1,char start_address_2,char quantity_register_1,char quantity_register_2);
enum modbus_function_status fc_write_single_register(char start_address_1,char start_address_2,char value_register_1,char value_register_2);
enum modbus_function_status fc_write_single_coil(char start_address_1,char start_address_2,char value_register_1,char value_register_2);
enum modbus_function_status fc_write_multiple_register(char read_bufor[]);
enum modbus_function_status fc_write_multiple_coil(char read_bufor[]);
enum modbus_function_status fc_update_firmaware(void);
unsigned short int CRC16 (const char *nData, unsigned short int wLength);
int get_device_address(void);
#endif /* MODBUS_H_ */
