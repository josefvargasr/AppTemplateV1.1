/*
 * NONIN_M1.c
 *
 * Created: 20/04/2015 13:00
 *  Author: Juan Lopera & Julián Otálvaro
 */ 

#include "NONIN_M1.h"

/*  */
uint8_t getPrefusionStatus(void){
	if (nonin_respond.ustatus.status.LPRF == 1){
		return LOW_PERFUSION;
	}
	else if (nonin_respond.ustatus.status.MPRF == 1){
		return MARGINAL_PERFUSION;
	}
	else{
		return 0;
	}
}

uint8_t nonin_m1_receiver(uint8_t rx_byte){
	//char SendChar_Alt[8];
	static uint8_t byte_counter = 0;
	switch (byte_counter){
		case 0:
			if((rx_byte & 0x80) == 0x80){
				nonin_status = rx_byte;
				byte_counter++;
			}
			return 0;
			break;
		
		case 1:
			nonin_hrByte = rx_byte;
			byte_counter++;
			return 0;
			break;
		
		case 2:
			nonin_Spo2Value = rx_byte;
			byte_counter = 0;
			return NONIN_STRUCT_FILLED;
			break;
		
		default:
			return 0;
		break;
	}
}


	