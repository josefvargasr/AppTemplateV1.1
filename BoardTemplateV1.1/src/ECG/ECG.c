/*
 * ECG.c
 *
 * Created: 22/09/2014 13:39:35
 *  Author: Jose Arboleda
 */ 


#include "ECG.h"
#include "ioport.h"
#include "stdio.h"

static bool user_led = false;

signed long calculate_wavelet(const signed long* mother_wavelet, signed long* sample_buffer, signed long ecg_data){
	
	static unsigned short actual_pos = 0; //Actual sample circular-buffer position
	unsigned short sample_index = 0;      //Sample buffer index for convolution
	signed long conv_out = 0;           //Convolution output
	signed int aux = 0;
	
	sample_buffer[actual_pos] =  ecg_data; //Save new sample
	
	if(actual_pos == (MOTHER_WAVELET_LENGTH - 1)){
		actual_pos = 0;
	}else{
		actual_pos++;
	}
	
	sample_index = actual_pos; //Initialize sample buffer index before convolution
	
	//Calculate convolution output for the actual sample
	for(unsigned short mother_index = 0; mother_index < MOTHER_WAVELET_LENGTH; mother_index++){
		conv_out = conv_out + (signed long)(sample_buffer[sample_index] * mother_wavelet[mother_index]);
		if(sample_index == (MOTHER_WAVELET_LENGTH - 1)){
			sample_index = 0;
		}else{
			sample_index++;
		}
	}
	return (conv_out/FLOAT_FIX);
	//return (aux >> 8);
}



signed long adjust_threshold(unsigned char th_buffer_index, signed long* threshold_buffer){
	signed long threshold_accum = 0;					//Threshold value accumulator
	signed long threshold_max = THRESHOLD_MAX;			//Initial maximum threshold 
	unsigned char buff_index = th_buffer_index;		//Index to iterate over threshold buffer
	signed long threshold_min = threshold_buffer[buff_index];	//Initial minimum value
	signed long threshold_aux = 0;						//Auxiliary value to operate the new threshold
	
	//Sum buffer values and look for max and min
	for(unsigned char i = 0; i < THRESHOLD_BUFF_SIZE; i++){
		
		threshold_accum = threshold_accum + threshold_buffer[buff_index];
		
		if(threshold_buffer[buff_index] >= threshold_max){
			threshold_max = threshold_buffer[buff_index];
		}
		if(threshold_buffer[buff_index] <= threshold_min){
			threshold_min = threshold_buffer[buff_index];
		}
		if(buff_index == (THRESHOLD_BUFF_SIZE - 1)){
			buff_index = 0;
		}else{
			buff_index++;
		}
	}
	
	//Apply the equation to adjust threshold
	threshold_aux = ((signed long) (threshold_accum - threshold_min - threshold_max)) / 4;
	threshold_aux = threshold_aux * 55;
	threshold_aux = threshold_aux / 100;
	return ((signed long)threshold_aux);
	//return (threshold_aux >> 8);
	
}


void ECG_process_state_machine(ECG_global_data* data, const signed long* mother_wavelet, signed long* sample_buffer, signed long* threshold_buffer){
	
	switch(data->ecg_state){
		
		case INIT_WAVELET:
			if(data->sample_counter > SIGNAL_SAMPLES_OFFSET){
				
				sample_buffer[data->sample_buffer_index] = data->ecg_data;
				(data->sample_buffer_index)++;
				
				if(data->sample_buffer_index == MOTHER_WAVELET_LENGTH){
					data->ecg_state = INIT_THRESHOLD;
				}
			}
		break;
		
		case INIT_THRESHOLD:
			data->swave = calculate_wavelet(mother_wavelet, sample_buffer, data->ecg_data);
			data->swave_counter++;
			if((data->swave) >= (data->swave_max)){
				data->swave_max = data->swave;
			}
			if(data->swave_counter >= NUMBER_OF_WAVELETS_TO_FIRST_THRESHOLD){
				signed long threshold_aux = 0;
				
				data->ecg_state = SEARCH_R_PEAK;
				threshold_aux = ((signed long) data->swave_max) * 4;
				threshold_aux = threshold_aux / 10;
				data->threshold = threshold_aux;
				
			}
		break;
		
		case WAIT_THRESHOLD_UP:
			data->swave = calculate_wavelet(mother_wavelet, sample_buffer, data->ecg_data);
			if(data->swave > data->threshold){
				data->swave_max = data->swave;
				data->ecg_state = SEARCH_R_PEAK;
			}
		break;
		
		case SEARCH_R_PEAK:
		    data->swave = calculate_wavelet(mother_wavelet, sample_buffer, data->ecg_data);
			
			if(data->swave >= data->swave_max){
				data->swave_max = data->swave;
			}else{
				//Max detected, generate event if needed
				//ioport_set_pin_level(PIN_PC08, true);
				
				if(user_led){
					//ioport_set_pin_level(PIN_PC07, false);
					user_led = false;
				}else{
					//ioport_set_pin_level(PIN_PC07, true);
					user_led = true;
				}
				
				//printf("R\r\n");
				
				threshold_buffer[data->threshold_buffer_index] = data->swave_max;
				data->threshold_buffer_index++;
				if(data->threshold_buffer_index > (THRESHOLD_BUFF_SIZE - 1)){
					data->threshold_buffer_index = 0;
					data->threshold_buffer_full =1;
				}
				data->ecg_state = WAIT_THRESHOLD_DOWN;
				//ioport_set_pin_level(PIN_PC08, false); //Reset pin
			}
		break;
		
		case WAIT_THRESHOLD_DOWN:
		    data->swave = calculate_wavelet(mother_wavelet, sample_buffer, data->ecg_data);

			if(data->swave < data->threshold){
				if(data->threshold_buffer_full == 1){
					data->threshold = adjust_threshold(data->threshold_buffer_index, threshold_buffer);
				}
				data->ecg_state = WAIT_THRESHOLD_UP;
			}
		break;
		
		default:
		break;
	}
	
}


void ECG_data_init(ECG_global_data* data){
	data->ecg_data = 0;
	data->ecg_state = INIT_WAVELET;
	data->sample_buffer_index = 0;
	data->sample_counter = 0;
	data->swave = 0;
	data->swave_counter = 0;
	data->swave_max = THRESHOLD_MAX;
	data->threshold = 0;
	data->threshold_buffer_full = 0;
	data->threshold_buffer_index = 0;
}
