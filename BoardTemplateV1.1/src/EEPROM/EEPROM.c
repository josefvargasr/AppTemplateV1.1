/*
 * CFile1.c
 *
 * Created: 06/04/2015 15:42:36
 *  Author: EMBEBIDOS_EJE3
 */ 
#include "EEPROM.h"

/*******************************************************************************
*	READ DATA FROM EEPROM
*	***** ARGUMENTS *****
*	/param reasAdd:		EEEPROM address from where it is wanted to start reading
*	/param data:		Pointer of a byte vector variable in which data read is going to be stored
*	/param dataSize:	Size of the vector variable pointed by "data" pointer
*
*	***** FUNCTIONALITY *****
*	readData() reads a frame of 0-137071 data, the start address is specified in "readAdd" argument,
*	then the function will query as many data as specified by sizeData from EEPROM and will store it 
*	in variable pointed by "data" pointer argument.
*
*	IMPORTANT: there's not knowledge about the limit of data it's allowed to be read in a frame 
*******************************************************************************/
void EEPROM_readData(uint8_t *readAdd, uint8_t *data, uint32_t dataSize){
	unsigned short r_data = 0;
	unsigned char r_pcs;
	
	spi_write(SPI, READ, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	/**************** SENDING ADDRESS ************************/ 
	spi_write(SPI, readAdd[2], spi_get_pcs(3),  0);				// Address MSB
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, readAdd[1], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, readAdd[0], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	
	/**************** RECEIVING DATA ************************/
	for(int i = 0; i<dataSize-1; i++){
		spi_write(SPI, 0xAA, spi_get_pcs(3),  0);
		while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
		spi_read(SPI, (uint16_t*) data, (uint8_t*) &r_pcs);	// It receive data in "data" and it's a pointer
		data++;		
	}
	spi_write(SPI, 0x55, spi_get_pcs(3),  1);				// The las byte must toggle CS
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	WRITE DATA FROM EEPROM
*	***** ARGUMENTS *****
*	/param reasAdd:		EEEPROM address where it is wanted to start writing
*	/param data:		Pointer of a byte vector variable in where data to be sent is
*	/param dataSize:	Size of the vector variable pointed by "data" pointer
*
*	***** FUNCTIONALITY *****
*	writeData() writes a frame of 0-137071 data, the start address is specified in "readAdd" argument,
*	then the function will store as many data as specified by sizeData in EEPROM and will store it
*	in the variable pointed by "data" pointer argument.
*
*	IMPORTANT: there's not knowledge about the limit of data it's allowed to be read in a frame
*******************************************************************************/
 void EEPROM_writeData(uint8_t *writeAdd, uint8_t *data, uint8_t dataSize){
	unsigned short r_data = 0;
	unsigned char r_pcs;
	 
	_EEPROM_wren();
	spi_write(SPI, WRITE, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	
	/**************** SENDING ADDRESS ************************/ 
	spi_write(SPI, writeAdd[2], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	 
	spi_write(SPI, writeAdd[1], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	 
	spi_write(SPI, writeAdd[0], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);	
	

	/**************** SENDING DATA ************************/
	for(int i = 0; i<dataSize-1; i++){
		spi_write(SPI, *data, spi_get_pcs(3),  0);					// It receive data in "data" and it's a pointer
		while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
		spi_read(SPI, (uint16_t*) r_data, (uint8_t*) &r_pcs);
		data++;
	}
	spi_write(SPI, *data, spi_get_pcs(3),  1);						// The las byte must toggle CS
	spi_read(SPI, (uint16_t*) r_data, (uint8_t*) &r_pcs);
	
	while( EEPROM_getWriteInProcess() );							// Deberíamos hacer esto aquí pa evitarle el complique al usuario
}

/*******************************************************************************
*	ENABLE WRITE EEPROM LATCH
*	_wren() must be called before any write operation 
*******************************************************************************/ 
void _EEPROM_wren(){
	unsigned short r_data = 0;
	unsigned char r_pcs;
	spi_write(SPI, WREN, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	DISABLE WRITE EEPROM LATCH
*******************************************************************************/
void _EEPROM_wrdi(){
	unsigned short r_data = 0;
	unsigned char r_pcs;
	spi_write(SPI, WRDI, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	READ EEPROM STATUS BYTE
*	| W/R|| _ | _ | _ | W/R | W/R |  R  |  R  |
*	| WPEN| X | X | X | BP1 | BP0 | WEL | WIP |
*******************************************************************************/
uint8_t _EEPROM_readStatusReg(){
	unsigned char r_pcs;
	uint8_t r_data = 0;
	
	spi_write(SPI, RDSR, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, 0x00, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	return r_data;
}

/*******************************************************************************
*	WRITE EEPROM STATUS BYTE
*	| W/R|| _ | _ | _ | W/R | W/R |  R  |  R  |
*	| WPEN| X | X | X | BP1 | BP0 | WEL | WIP |
*******************************************************************************/
void _EEPROM_writeStatusReg(uint8_t w_status){
	unsigned char r_pcs;
	uint8_t r_data = 0;
	
	spi_write(SPI, WRSR, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, w_status, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	The Chip Erase function will erase all bits (0xFF) in the array. 
*
*	While the device is executing the chipErase() function, the WriteInProcess() 
*	macro can be read to determine when the Chip Erase function is complete.
*
*******************************************************************************/
void EEPROM_chipErase(){
	unsigned short r_data = 0;
	unsigned char r_pcs;
	
	_EEPROM_wren();
	spi_write(SPI, CE, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	DEEP POWER DOWN MODE
*******************************************************************************/
void EEPROM_powerDownMode(){
	unsigned short r_data = 0;
	unsigned char r_pcs;
	spi_write(SPI, DPD, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	RELEASE EEPROM FROM POWER DOWN MODE
*******************************************************************************/
void EEPROM_releasePowerDownMode(){
	(void) EEPROM_readVendorId();
}

/*******************************************************************************
*	It returns 8 bits vendor ID
*******************************************************************************/
uint8_t EEPROM_readVendorId(){
	
	unsigned short r_data = 0;
	unsigned char r_pcs;
	
	spi_write(SPI, RDID, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	/**************** SENDING DUMMY ADDRESS ************************/
	spi_write(SPI, 0x00, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, 0x00, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, 0x00, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	/**************** RECEIVING DATA ************************/
	spi_write(SPI, 0x05, spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	return r_data;
}

/*******************************************************************************
*	ERRASE A WHOLE PAGE OF DATA FROM EPPROM 
*	\param pageAdd: you can write any of the 256 address into this function and it will erase the whole page
*******************************************************************************/
void EEPROM_pageErase(uint8_t *pageAdd){	
		
	unsigned short r_data = 0;
	unsigned char r_pcs;
	//Command
	spi_write(SPI, PE, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	//address
	spi_write(SPI, pageAdd[2], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, pageAdd[1], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, pageAdd[0], spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}

/*******************************************************************************
*	ERRASE A WHOLE SECTOR OF DATA FROM EPPROM
*	\param pageAdd: you can write any of the 65535 address into this function and it will erase the whole sector
*******************************************************************************/
void EEPROM_sectorErase(uint8_t *sectorAdd){
	
	unsigned short r_data = 0;
	unsigned char r_pcs;
	//Command
	spi_write(SPI, SE, spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	//address
	
	spi_write(SPI, sectorAdd[2], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, sectorAdd[1], spi_get_pcs(3),  0);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	
	spi_write(SPI, sectorAdd[0], spi_get_pcs(3),  1);
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
}