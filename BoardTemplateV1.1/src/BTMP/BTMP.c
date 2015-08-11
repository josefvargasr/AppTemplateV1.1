/*
 * BTMP.c
 *
 * Created: 22/06/2015 11:45:34 a.m.
 *  Author: Juanes
 */ 

#include "BTMP.h"

twi_package_t packet_tx, packet_rx;						//! TWI data package

//#define PATTERN_TEST_LENGTH (sizeof(write_data)/sizeof(uint8_t))
//static uint8_t write_data[] = {0xB5, 0x9F, 0x00};		//! Array to store the test for sending
//static uint8_t read_data[PATTERN_TEST_LENGTH];			//! Array to store the received data

/***************************************************************
							CRC-8
****************************************************************/
uint8_t Crc8(const void *vptr, int len)
{
	const uint8_t *data = vptr;
	unsigned crc = 0;
	int i, j;
	for (j = len; j; j--, data++) {
		crc ^= (*data << 8);
		for(i = 8; i; i--) {
			if (crc & 0x8000)
			crc ^= (0x1070 << 3);
			crc <<= 1;
		}
	}
	return (uint8_t)(crc >> 8);
}
/***************************************************************
					PRIMITIVE FUNCTIONS
****************************************************************/
uint8_t * btmp_read(uint8_t addr)
{
	uint8_t read_data[PATTERN_LENGTH];
	status_code_t status;
	read_data[0] = 0;
	packet_rx.chip = BTMP_TARGET_ADDRESS;					// TWI chip address to communicate with 
	packet_rx.addr_length = BTMP_TARGET_ADDR_LGT;			// Length of the TWI data address segment (1-3 bytes) 
	packet_rx.length = PATTERN_LENGTH;						// How many bytes do we want to write 
	packet_rx.addr[0] = addr;								// TWI address/commands to issue to the other chip (node)
	packet_rx.buffer = read_data;							// Where to find the data to be written 
	status = twi_master_read(EXAMPLE_TWIM, &packet_rx);		// Read data from TARGET 
	
	int8_t frame[]= {0xB4, addr, 0xB5, read_data[0], read_data[1]};		//making frame to confirm crc8
	
	if (status == STATUS_OK && Crc8(frame, 5) == read_data[2])			//
		return read_data;
	else 
		return NULL;
}

status_code_t btmp_write(uint8_t addr, uint8_t *data)
{
	packet_tx.chip = BTMP_TARGET_ADDRESS;					// TWI chip address to communicate with
	packet_tx.addr_length = BTMP_TARGET_ADDR_LGT;			// Length of the TWI data address segment (1-3 bytes)
	packet_tx.addr[0] = addr;								// TWI address/commands to issue to the other chip (node)
	packet_tx.length = PATTERN_LENGTH;						// How many bytes do we want to write
	packet_tx.buffer = (void *) data;						// Where to find the data to be written
	return twi_master_write(EXAMPLE_TWIM, &packet_tx);		// Write data to TARGET
}
/***************************************************************
					HIGH LEVEL FUNCTIONS
****************************************************************/

float btmp_get_temperature()
{
	uint8_t *data = NULL;
	int tag;
	data = btmp_read(BTMP_AMBIENT_TEMP_ADDR);
	if (data != NULL)
	{
		tag = ( (data[1]<<8) + data[0] );
		return tag*0.02 - 273.15;
	} else {
		return BTMP_ERROR;
	}
}

float btmp_get_temperature_ob1()
{
	uint8_t *data = NULL;
	int tag;
	data = btmp_read(BTMP_OB1_TEMP_ADDR);
	if (data != NULL)
	{
		tag = ( (data[1]<<8) + data[0] );
		return tag*0.02 - 273.15;
	} else {
		return BTMP_ERROR;
	}
}


 

//#include "BTMP.h"
//
//#define PATTERN_TEST_LENGTH (sizeof(write_data)/sizeof(uint8_t))
//static uint8_t write_data[] = {0xB5, 0x9F, 0x00};		//! Array to store the test data for sending
//
//static uint8_t read_data[PATTERN_TEST_LENGTH];			//! Array to store the received test data
//
//twi_package_t packet_tx, packet_rx;				//! TWI data package
//
///***************************************************************
							//CRC-8
//****************************************************************/
//uint8_t Crc8(const void *vptr, int len)
//{
	//const uint8_t *data = vptr;
	//unsigned crc = 0;
	//int i, j;
	//for (j = len; j; j--, data++) {
		//crc ^= (*data << 8);
		//for(i = 8; i; i--) {
			//if (crc & 0x8000)
			//crc ^= (0x1070 << 3);
			//crc <<= 1;
		//}
	//}
	//return (uint8_t)(crc >> 8);
//}
///***************************************************************
					//PRIMITIVE FUNCTIONS
//****************************************************************/
//uint8_t * btmp_read(uint8_t addr)
//{
	//status_code_t status;
	//read_data[0] = 0;
	//packet_rx.chip = BTMP_TARGET_ADDRESS;					// TWI chip address to communicate with 
	//packet_rx.addr_length = BTMP_TARGET_ADDR_LGT;			// Length of the TWI data address segment (1-3 bytes) 
	//packet_rx.length = 3; //PATTERN_TEST_LENGTH				// How many bytes do we want to write 
	//packet_rx.addr[0] = addr;								// TWI address/commands to issue to the other chip (node)
	//packet_rx.buffer = read_data;							// Where to find the data to be written 
	//status = twi_master_read(EXAMPLE_TWIM, &packet_rx);		// Read data from TARGET 
	//
	//int8_t frame[]= {0xB4, addr, 0xB5, read_data[0], read_data[1]};		//making frame to confirm crc8
	//
	//if (status == STATUS_OK && Crc8(frame, 5) == read_data[2])			//
		//return read_data;
	//else 
		//return NULL;
//}
//
//status_code_t btmp_write(uint8_t addr, uint8_t *data)
//{
	//packet_tx.chip = BTMP_TARGET_ADDRESS;					// TWI chip address to communicate with
	//packet_tx.addr_length = BTMP_TARGET_ADDR_LGT;			// Length of the TWI data address segment (1-3 bytes)
	//packet_tx.addr[0] = addr;								// TWI address/commands to issue to the other chip (node)
	//packet_tx.length = 1; //PATTERN_TEST_LENGTH				// How many bytes do we want to write
	//packet_tx.buffer = (void *) data;						// Where to find the data to be written
	////printf("Writing data to TARGET\r\n");
	//return twi_master_write(EXAMPLE_TWIM, &packet_tx);		// Write data to TARGET
//}
///***************************************************************
					//HIGH LEVEL FUNCTIONS
//****************************************************************/
//
//float btmp_get_temperature()
//{
	//uint8_t *data = NULL;
	//int tag;
	//data = btmp_read(BTMP_AMBIENT_TEMP);
	//if (data != NULL)
	//{
		//tag = ( (data[1]<<8) + data[0] );
		//return tag*0.02 - 273.15;
	//} else {
		//return BTMP_ERROR;
	//}
//}
//
//float btmp_get_temperature_ob1()
//{
	//uint8_t *data = NULL;
	//int tag;
	//data = btmp_read(0x07);
	//if (data != NULL)
	//{
		//tag = ( (data[1]<<8) + data[0] );
		//return tag*0.02 - 273.15;
	//} else {
		//return BTMP_ERROR;
	//}
//}

