/*
 * ACCL.h
 *
 * Created: 24/06/2015 10:27:51 a.m.
 *  Author: Juanes
 */ 


#ifndef ACCL_H_
#define ACCL_H_

#include <asf.h>

/** TWIM Module Used */
#define EXAMPLE_TWIM         TWIM1
/** Target's TWI address */
#define ACCL_TARGET_ADDRESS       0x1C
/** Internal Address length */
#define ACCL_TARGET_ADDR_LGT      1
/** Internal Address */
#define VIRTUALMEM_ADDR      0x0E0000

#define PATTERN_LENGTH	 1

uint8_t * accl_read(uint8_t addr);
status_code_t accl_write(uint8_t addr, uint8_t *data);



///** TWIM Interrupt Number */
//#define EXAMPLE_TWIM_IRQn    TWIM1_IRQn
///** Speed of TWI */
//#define TWIM_MASTER_SPEED    TWI_STD_MODE_SPEED
///** TWIM Interrupt Handler */
//#define EXAMPLE_TWIM_Handler TWIM1_Handler




#endif /* ACCL_H_ */