/*
 * CFile1.c
 *
 * Created: 24/06/2015 10:27:31 a.m.
 *  Author: Juanes
 */ 

#include "ACCL.h"

//#define PATTERN_TEST_LENGTH (sizeof(write_data)/sizeof(uint8_t))
static uint8_t write_data[] = {0x00};		//! Array to store the test data for sending

static uint8_t read_data[1];			//! Array to store the received test data

twi_package_t packet_tx, packet_rx;				//! TWI data package

/*
*	\return pointer to data if read was successful, NULL otherwise
*/
uint8_t * accl_read(uint8_t addr)
{
	status_code_t status;
	read_data[0] = 0;
	packet_rx.chip = ACCL_TARGET_ADDRESS;						// TWI chip address to communicate with
	packet_rx.addr_length = ACCL_TARGET_ADDR_LGT;				// Length of the TWI data address segment (1-3 bytes)
	packet_rx.length = 1; //PATTERN_TEST_LENGTH				// How many bytes do we want to write
	packet_rx.addr[0] = addr;								// TWI address/commands to issue to the other chip (node)
	packet_rx.buffer = read_data;							// Where to find the data to be written
	status = twi_master_read(EXAMPLE_TWIM, &packet_rx);		// Read data from TARGET
	
	if (status == STATUS_OK)
		return read_data;
	else
		return NULL;
}

/**
 * \brief Write data to accelerometer module the module.
 *
 * \return STATUS_OK   if the write succeeds, error code otherwise.
 */
status_code_t accl_write(uint8_t addr, uint8_t *data)
{
	packet_tx.chip = ACCL_TARGET_ADDRESS;					// TWI chip address to communicate with
	packet_tx.addr_length = ACCL_TARGET_ADDR_LGT;			// Length of the TWI data address segment (1-3 bytes)
	packet_tx.addr[0] = addr;								// TWI address/commands to issue to the other chip (node)
	packet_tx.length = 1; //PATTERN_TEST_LENGTH				// How many bytes do we want to write
	packet_tx.buffer = (void *) data;						// Where to find the data to be written
	//printf("Writing data to TARGET\r\n");
	return twi_master_write(EXAMPLE_TWIM, &packet_tx);		// Write data to TARGET
}