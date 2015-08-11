/*
 * NONIN_M1.h
 *
 * Created: 20/04/2015 13:00
 * Author: Juan Lopera & Julián Otálvaro
 */ 

#ifndef NONIN_M1_H_
#define NONIN_M1_H_

#include <asf.h>
#include "usart_spi.h"

/************************************* Initial considerations and use of this library ************************************************/


#define SENSOR_DISCONNECTED		1
#define SENSOR_CONNECTED		0
#define OUT_OF_TRACK			1
#define LOW_PERFUSION			1
#define MARGINAL_PERFUSION		2
#define ARTIFACT				1
#define NONIN_STRUCT_FILLED		1


#define nonin_hrValue				(uint16_t)(nonin_respond.ustatus.status.HR_H << 7) + nonin_respond.uhr.hr.HR_L
#define nonin_Spo2Value				nonin_respond.nonin_byte
#define nonin_status				nonin_respond.ustatus.status_byte
#define nonin_outOfTrack			nonin_respond.ustatus.status.OOT
#define nonin_conectionStatus()		nonin_respond.ustatus.status.SNSD
#define nonin_PulseBeatStatus()		nonin_respond.ustatus.status.ARTF
#define nonin_hrByte				nonin_respond.uhr.hr_byte			//solo usar para ingresar el valor en la estructura

// Structure to handle frame bytes
typedef struct {
	
	/* Structure to handle Status-Byte bits */
	union{
		struct {
			uint8_t HR_H:2;
			uint8_t ARTF:1;
			uint8_t MPRF:1;
			uint8_t LPRF:1;
			uint8_t OOT:1;
			uint8_t SNSD:1;
			uint8_t SYNC:1;
		}status;
		uint8_t status_byte;
	}ustatus;
	
	/* Structure to handle Heart Rate-Byte bits */
	union{
		struct {
			uint8_t HR_L:7; //Cambié el orden PILAS!!
			uint8_t SYNC:1;
		}hr;
		uint8_t hr_byte;
	}uhr;
	
	/* SpO2 byte */
	uint8_t nonin_byte;
	
}nonin_struct;

nonin_struct nonin_respond;
//extern signed char byte_counter;

uint8_t getPrefusionStatus(void);
uint8_t nonin_m1_receiver(uint8_t rx_byte);

#endif /* NONIN_M1_H_ */