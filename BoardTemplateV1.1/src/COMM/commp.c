/*
 * commp.c
 *
 * Created: 16/03/2015 2:05:59 p. m.
 *  Author: Jose Fernando
 */ 
#include "commp.h"

//Function to send an ECG data frame
bool send_ecg_data(char* ecg_buffer, unsigned short b_index, unsigned char sec, Usart* p_usart){
	usart_putchar(p_usart, FRAME_HEAD);
	usart_putchar(p_usart, ECG_TYPE);
	usart_putchar(p_usart, sec);
	
	for(unsigned char i = 0; i < ECG_PAYLOAD_LENGTH; i++){
		usart_putchar(p_usart, ecg_buffer[buff_index_control(ECG_BUFFER_LENGTH, b_index, i)]);
	}
	
	usart_putchar(p_usart, FRAME_TAIL);
	
	return true;
}

//Function to control circular buffer access
unsigned short buff_index_control(unsigned short x_size, unsigned short n, signed int next_pos){
	signed int next = 0;
	if(abs(next_pos) < x_size){
		next = n + next_pos;
		if(next >= x_size){
			next = next - x_size;
			return (unsigned short)next;
			
		}else if(next < 0){
			next = x_size + next;
			return (unsigned short)next;
		}else{
			return (unsigned short)next;
		}
	}else{
		return;
	}
}

//Function to flush the ECG data buffer
void ecg_buffer_flush(struct ECG_buffer* s_buffer){
	s_buffer->buffer_index = 0;
	s_buffer->full_flag = false;
}

//Function to control index overflow 
void ecg_fill_index_overflow_ctrl(struct ECG_buffer* s_buffer){
	s_buffer->buffer_index = s_buffer->buffer_index + SAMPLE_SIZE;
	if(s_buffer->buffer_index == ECG_BUFFER_LENGTH){
		s_buffer->buffer_index = 0;
		s_buffer->full_flag = true;
	}
}

//Function to send the variables
bool send_variables_data(uint8_t* data_buffer, Usart* p_usart){
	usart_putchar(p_usart, FRAME_HEAD);
	usart_putchar(p_usart, VAR_TYPE);
	
	for(unsigned short i = 0; i < VAR_PAYLOAD_LENGTH; i++){
		usart_putchar(p_usart, data_buffer[i]);
	}
	
	usart_putchar(p_usart, FRAME_TAIL);
	
	return true;
}

//Function to load a test signal on the buffer
void load_test_signal(struct ECG_buffer* s_buffer){
	
	for(unsigned short i = 0; i < ECG_BUFFER_LENGTH; i++){
		s_buffer->buffer[i] = i % ECG_PAYLOAD_LENGTH;
	}
	s_buffer->full_flag = true;
}