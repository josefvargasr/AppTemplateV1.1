/*
 * NONIN_M2.h
 *
 * Created: 25/03/2015 11:32:45
 *  Author: EMBEBIDOS_EJE3
 */ 


#ifndef NONIN_M2_H_
#define NONIN_M2_H_

#include <asf.h>

/*******************************************************************************
*								ALERT MACROS
*******************************************************************************/
#ifndef NONIN_STRUCT_FILLED

#define SENSOR_DISCONNECTED		1
#define SENSOR_CONNECTED		0
#define OUT_OF_TRACK			1
#define ARTIFACT				1
#define NONIN_STRUCT_FILLED		1
#define GREEN_PERFUSION			1
#define RED_PERFUSION			2
#define YELLOW_PERFUSION		3

#endif

/*******************************************************************************
*					NONIN mode 2 Frame handler Macros
*******************************************************************************/

#define BYTE_1					_nonin_respond_mode_2.currentByte_1
#define STATUS					_nonin_respond_mode_2.uStatus.statusByte
#define PLETH(N_BYTE)			_nonin_respond_mode_2.pleth[N_BYTE]
#define BYTE_4(N_BYTE)			_nonin_respond_mode_2.uMeasures.Byte_4[N_BYTE]

#define noninM2_hrValue			(uint16_t)(_nonin_respond_mode_2.uMeasures.uByte_4.hr[0] << 7) + _nonin_respond_mode_2.uMeasures.uByte_4.hr[1]
#define noninM2_spo2Value		_nonin_respond_mode_2.uMeasures.uByte_4.SpO2

/*******************************************************************************
*						NONIN mode 2 Status handler Macros
*******************************************************************************/
#define noninM2_conectionStatus()		_nonin_respond_mode_2.uStatus.status.SNSD
#define noninM2_sensorAlarm()			_nonin_respond_mode_2.uStatus.status.SNSA
#define noninM2_askForSync()			_nonin_respond_mode_2.uStatus.status.SYNC
#define noninM2_pulseBeatStatus()		_nonin_respond_mode_2.uStatus.status.ARTF
#define noninM2_outOfTrack				_nonin_respond_mode_2.uStatus.status.OOT
#define GPRF2							_nonin_respond_mode_2.uStatus.status.GPRF
#define RPRF2							_nonin_respond_mode_2.uStatus.status.RPRF

/*******************************************************************************
*								DATA STRUCTURE
*******************************************************************************/
typedef struct {
	
	uint8_t currentByte_1;		/* Just for checksum */
	uint8_t pleth[25];			/* Plethysmographic wave */
	union{						/* Structure to handle fourth packet frame byte  */
		struct {
			uint8_t hr[2];
			uint8_t SpO2;
			uint8_t REV;
			uint8_t reserved_1[4];
			uint8_t nonin_D;
			uint8_t nonin_Fast;
			uint8_t nonin_BB;
			uint8_t reserved_2[2];
			uint8_t E_HR[2];
			uint8_t E_SpO2;
			uint8_t E_nonin_D;
			uint8_t reserved_3[2];
			uint8_t HR_D[2];
			uint8_t E_HR_D[2];
			uint8_t reserved_4[2];
		}uByte_4;
		uint8_t Byte_4[25];
	}uMeasures;
	union{						/* Structure to handle the status bits */
		struct {
			uint8_t SYNC:1;
			uint8_t GPRF:1;
			uint8_t RPRF:1;
			uint8_t SNSA:1;
			uint8_t OOT:1;
			uint8_t ARTF:1;
			uint8_t SNSD:1;
			uint8_t :1;
		}status;
		uint8_t statusByte;
	}uStatus;
}nonin_struct_mode_2;

/* Structure Instance */
nonin_struct_mode_2 _nonin_respond_mode_2;

/*******************************************************************************
*								METHODS
*******************************************************************************/
uint8_t chekSum(int index);
uint8_t get_m2_PrefusionStatus();
uint8_t nonin_m2_receiver(uint8_t rx_byte);


#endif /* NONIN_M2_H_ */