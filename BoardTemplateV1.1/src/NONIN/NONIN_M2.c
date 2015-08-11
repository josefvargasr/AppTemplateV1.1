/*
 * NONIN_M2.c
 *
 * Created: 25/03/2015 11:32:30
 *  Author: EMBEBIDOS_EJE3
 */ 
#include "NONIN_M2.h"


uint8_t chekSum(int index){
	return (BYTE_1 + STATUS + PLETH(index) + BYTE_4(index)) % 256;
}

uint8_t get_m2_PrefusionStatus(){
	if (_nonin_respond_mode_2.uStatus.status.GPRF & _nonin_respond_mode_2.uStatus.status.RPRF){
		return YELLOW_PERFUSION;
	} else if (_nonin_respond_mode_2.uStatus.status.GPRF == 1){
		return GREEN_PERFUSION;
	} else if (_nonin_respond_mode_2.uStatus.status.RPRF == 1){
		return RED_PERFUSION;
	}
	else{
		return 0;
	}
}

uint8_t nonin_m2_receiver(uint8_t rx_byte){
	char SendChar_Alt[8];
	static uint8_t Start = 0;
	/* Frame Handlers */
	static uint8_t byte_counter = 0;
	static uint8_t index;
	static uint8_t currentByte;
	
	/* Task Begin */
	if (rx_byte == 0x01 && !Start){
		Start = 1;
		byte_counter = 0;
	}
	if (byte_counter == 125) byte_counter = 0;		// if (byte_counter == 125 || notSync)
	
	if (Start){
		uint8_t currentByte = (byte_counter) % 5;
		uint8_t index = (uint8_t)byte_counter/5;
		
		switch(currentByte){
			case 0:
				if (rx_byte == 0x01) BYTE_1 = rx_byte;
				else Start = 0;
				byte_counter++;
				return 0;
			
			case 1:
				STATUS = rx_byte;
				if (noninM2_askForSync() && index == 0){	// Si ocurre evento de inicio de paquete y además el index = 0 hay sincronía
					usart_write_line(USART1, "SYNC");
				} 
				else if ( !(!noninM2_askForSync() && !(index == 0)) ) Start = 0;	// que no ocurran ambos eventos a la vez es normal
				//if ((noninM2_askForSync() || index == 0) && !(noninM2_askForSync() && index == 0))  Start = 0;   //otra opción
				byte_counter++;
				return 0;
			
			case 2:
				PLETH(index) = rx_byte;
				byte_counter++;
				return 0;
			
			case 3:
				BYTE_4(index) = rx_byte;
				byte_counter++;
				return 0;
			
			case 4:
				byte_counter++;
				if (index == 24) return NONIN_STRUCT_FILLED;
				else return 0;
		}
	}/* (End Start) End Task */
}
