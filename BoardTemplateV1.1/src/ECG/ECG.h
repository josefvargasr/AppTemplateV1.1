/*
 * ECG.h
 *
 * Created: 22/09/2014 13:40:10
 *  Author: Jose Arboleda
 */ 


#ifndef ECG_H_
#define ECG_H_

//Constants and definitions
#define SIGNAL_SAMPLES_OFFSET 30#define MOTHER_WAVELET_LENGTH 27#define	NUMBER_OF_WAVELETS_TO_FIRST_THRESHOLD 512#define THRESHOLD_BUFF_SIZE 6
#define FLOAT_FIX 100
#define THRESHOLD_MAX 0x8000

typedef enum{      //ECG state machine states
	INIT_WAVELET,
	INIT_THRESHOLD,
	WAIT_THRESHOLD_UP,
	SEARCH_R_PEAK,
	WAIT_THRESHOLD_DOWN
	} ECG_p_state;
	
typedef enum{
	SENSOR_IDLE_STATE,
	SENSOR_ACTIVE_STATE,
	SENSOR_SENDING
	}sensor_state;

typedef struct ECG_global  
{
	ECG_p_state ecg_state;
	unsigned long sample_counter;
	unsigned short sample_buffer_index;
	signed long ecg_data;
	signed long swave;
	signed long swave_max;
	unsigned short swave_counter;
	signed long threshold; //signed long threshold;
	unsigned char threshold_buffer_index;
	unsigned char threshold_buffer_full;
} ECG_global_data;

//Function prototypes
signed long calculate_wavelet(const signed long* mother_wavelet, signed long* sample_buffer, signed long ecg_data);
signed long adjust_threshold(unsigned char th_buffer_index, signed long* threshold_buffer);
void ECG_process_state_machine(ECG_global_data* data, const signed long* mother_wavelet, signed long* sample_buffer, signed long* threshold_buffer);
void ECG_data_init(ECG_global_data* data);




#endif /* ECG_H_ */