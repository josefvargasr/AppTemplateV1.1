/*
 * commp.h
 *
 * Created: 16/03/2015 2:05:38 p. m.
 *  Author: Jose Fernando
 */ 


#ifndef COMMP_H_
#define COMMP_H_

//Includes
#include "usart.h"
#include "stdlib.h"

//Constants and definitions
#define FRAME_HEAD 0xFF
#define FRAME_TAIL 0xFA
#define ECG_PAYLOAD_LENGTH 75
#define VAR_PAYLOAD_LENGTH 10
#define CONTROL_PAYLOAD_LENGTH 2
#define ECG_BUFFER_LENGTH 7500
#define ECG_PACKETS_TX 100
#define SAMPLE_SIZE 3
#define ECG_REQUEST 0x53
#define VAR_REQUEST 0X56
#define SENSOR_ERROR 0x45 


enum{
	DUMMY_TYPE,
	ECG_TYPE,
	VAR_TYPE,
	CTRL_TYPE
	};

//Structs
struct ECG_buffer{
	char buffer[ECG_BUFFER_LENGTH];
	unsigned short buffer_index;
	bool full_flag;
	};

//Function prototypes
bool send_ecg_data(char* ecg_buffer, unsigned short b_index, unsigned char sec, Usart* p_usart);
unsigned short buff_index_control(unsigned short x_size, unsigned short n, signed int next_pos);
void ecg_buffer_flush(struct ECG_buffer* s_buffer);
void ecg_fill_index_overflow_ctrl(struct ECG_buffer* s_buffer);
bool send_variables_data(uint8_t* data_buffer, Usart* p_usart);
void load_test_signal(struct ECG_buffer* s_buffer);


#endif /* COMMP_H_ */