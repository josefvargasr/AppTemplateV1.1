/*
 * EEPROM.h
 *
 * Created: 06/04/2015 15:43:43
 *  Author: EMBEBIDOS_EJE3
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <asf.h>
#include "delay.h"
#include "spi.h"
#include "usart_spi.h"

/*
*		  EEPROM 25AA1024
*		   _____________
*		  |             |
*		--|	CS	   3.3V |--
*		  |				|
*		--|	MISO   3.3V	|--
*		  |				|
*		--|	3.3V	CLK |--
*		  |				|
*		--|	GND		MOSI|--
*		  |_____________|
*/

/********************************************************************************
*						EEPROM SPI INSTRUCTIONS
*********************************************************************************/
#define WRITE					0x02		// Write data to memory array beginning at selected addressRead data from memory array beginning at selected address
#define READ					0x03		// Read data from memory array beginning at selected address
#define RDSR					0x05		// Read STATUS register
#define WREN					0x06		// Set the write enable latch (enable write operations)
#define RDID					0xAB		// Release from Deep power-down and read electronic signature
#define WRDI					0x04		// Reset the write enable latch (disable write operations)
#define WRSR					0x01		// Write STATUS register
#define DPD						0xB9		// Deep Power-Down mode
#define RDID					0xAB		// release from power-down mode and read electronic signature
#define PE						0x42		// Page erase
#define SE						0xD8		// Sector erase
#define CE						0xC7		// Chip erase

/********************************************************************************
*							STATUS BYTE MACROS
*********************************************************************************/
#define	WIP_MASK				0x01		// Write-In-Process
#define WEL_MASK				0x02		// Write Enable Latch
#define BP0_MASK				0x04		// Block Protection 0 
#define BP1_MASK				0x08		// Block Protection 1
//#define WPEN_MASK				0x80		// Write-Protect Enable

#define EEPROM_getWriteInProcess()		_EEPROM_readStatusReg() & WIP_MASK	//It returns 1 if is a write operation in progress, 0 otherwise
#define	EEPROM_getWriteEnable()			_EEPROM_readStatusReg() & WEL_MASK	//It returns 1 if the write latch is enabled, 0 otherwise
#define	EEPROM_getBlockProt0()			_EEPROM_readStatusReg() & BP0_MASK	//It returns 1 if the BP0 is enabled, 0 otherwise
#define	EEPROM_getBlockProt1()			_EEPROM_readStatusReg() & BP1_MASK	//It returns 1 if the BP1 is enabled, 0 otherwise
#define EEPROM_enableBlockProt0()		_EEPROM_readStatusReg(BP0_MASK)	//Protects block 0
#define EEPROM_enableBlockProt1()		_EEPROM_readStatusReg(BP1_MASK)	//protects block 1

//uint8_t add[3];

/********************************************************************************
*								METHODS
*********************************************************************************/
void releasePowerDownMode();
uint8_t EEPROM_readVendorId();
void _EEPROM_wren();
void _EEPROM_wrdi();
void EEPROM_readData(uint8_t *readAdd, uint8_t *data, uint32_t dataSize);
void EEPROM_writeData(uint8_t *writeAdd, uint8_t *data, uint8_t dataSize);
uint8_t _EEPROM_readStatusReg();
void _EEPROM_writeStatusReg(uint8_t w_status);
void EEPROM_powerDownMode();
void EEPROM_pageErase(uint8_t *pageAdd);
void EEPROM_pageErase(uint8_t *sectorAdd);		//any address inside the sector to be erased is a valid address
void EEPROM_chipErase();

#endif /* EEPROM_H_ */