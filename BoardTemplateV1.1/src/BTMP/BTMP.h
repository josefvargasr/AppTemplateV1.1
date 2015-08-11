/*
 * BTMP.h
 *
 * Created: 22/06/2015 11:45:52 a.m.
 *  Author: Juanes
 */ 


#ifndef BTMP_H_
#define BTMP_H_

#include "twim.h"

/****************************************
	  TRANSFER CONFIGURATIONS
****************************************/
#define EXAMPLE_TWIM				TWIM1			// TWIM Module Used
#define BTMP_TARGET_ADDRESS			0x5A			// Target's TWI address
#define BTMP_TARGET_ADDR_LGT		1				// Internal Address length
#define PATTERN_LENGTH				3
//#define VIRTUALMEM_ADDR				0x250000		// Internal Address

/****************************************
				COMMANDS
****************************************/
#define BTMP_RAM_ACCESS_MASK		0x00
#define	BTMP_EEPROM_ACCESS_MASK		0x20
#define	BTMP_READ_FLAG_MASK			0xF0
#define BTMP_ENTER_SLEEP_MODE_MASK	0xFF

#define BTMP_ERROR					0x8000

/****************************************
				RAM MACROS
****************************************/
#define BTMP_AMBIENT_TEMP_ADDR		0x06
#define BTMP_OB1_TEMP_ADDR			0x07	
 
/****************************************
			LOW LEVEL FUNCTIONS
****************************************/
#define BTMP_INTERNAL_ADDR_MASK		0x1F
#define	btmp_ram_acces(addr)		RAM_ACCESS_MASK | (addr & INTERNAL_ADDR_MASK)
#define btmp_eeprom_acces(addr)		EEPROM_ACCESS_MASK | (addr & INTERNAL_ADDR_MASK)

	/* How to use it*/
//cmd = btmp_ram_acces(addr);
//btmp_read(cmd);

uint8_t Crc8(const void *vptr, int len);
uint8_t * btmp_read(uint8_t addr);
status_code_t btmp_write(uint8_t addr, uint8_t *data);

float btmp_get_temperature();
float btmp_get_temperature_ob1();

#endif /* BTMP_H_ */







/*
 * BTMP.h
 *
 * Created: 22/06/2015 11:45:52 a.m.
 *  Author: Juanes
 */ 

//
//#ifndef BTMP_H_
//#define BTMP_H_
//
////#include <asf.h>
//#include "twim.h"
//
//
///** TWIM Module Used */
//#define EXAMPLE_TWIM         TWIM1
///** Target's TWI address */
//#define BTMP_TARGET_ADDRESS       0x5A
///** Internal Address length */
//#define BTMP_TARGET_ADDR_LGT      1
///** Internal Address */
//#define VIRTUALMEM_ADDR      0x250000
//
//#define PATTERN_LENGTH	 3
//
///****************************************
				//COMMANDS
//****************************************/
//#define BTMP_RAM_ACCESS_MASK		0x00
//#define	BTMP_EEPROM_ACCESS_MASK		0x20
//#define	BTMP_READ_FLAG_MASK			0xF0
//#define BTMP_ENTER_SLEEP_MODE_MASK	0xFF
//
//
//#define BTMP_AMBIENT_TEMP			0x07 
//
///*** INTERNAL DEVICES ***/
//#define BTMP_EEPROM					0
//#define BTMP_RAM					1
//
//#define BTMP_ERROR					0x8000
//
///****************************************
			//LOW LEVEL FUNCTIONS
//****************************************/
//#define BTMP_INTERNAL_ADDR_MASK		0x1F
//#define	btmp_ram_acces(addr)		RAM_ACCESS_MASK | (addr & INTERNAL_ADDR_MASK)
//#define btmp_eeprom_acces(addr)		EEPROM_ACCESS_MASK | (addr & INTERNAL_ADDR_MASK)
//
//#define ADDR						0x07
//
////cmd = RAM_ACCESS_MASK | (addr & INTERNAL_ADDR_MASK);
////cmd = EEPROM_ACCESS_MASK | (addr & INTERNAL_ADDR_MASK);
//
//uint8_t Crc8(const void *vptr, int len);
//uint8_t * btmp_read(uint8_t addr);
//status_code_t btmp_write(uint8_t addr, uint8_t *data);
//
//float btmp_get_temperature();
//float btmp_get_temperature_ob1();
//
//#endif /* BTMP_H_ */
//
/////** TWIM Interrupt Number */
////#define EXAMPLE_TWIM_IRQn    TWIM3_IRQn
/////** Speed of TWI */
////#define TWIM_MASTER_SPEED    TWI_STD_MODE_SPEED
/////** TWIM Interrupt Handler */
////#define EXAMPLE_TWIM_Handler TWIM3_Handler