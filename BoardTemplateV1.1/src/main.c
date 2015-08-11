/**
 * \file
 *
 * \brief Empty user application template
 *
 */
/*************************************************************************************************************
										Application and ASF includes
*************************************************************************************************************/
#include <asf.h>
#include <ECG/ECG.h>
#include <ADS/ADS.h>
#include <COMM/commp.h>
#include <NONIN/NONIN_M1.h>
#include <BTMP/BTMP.h>
#include <EEPROM/EEPROM.h>
//
///*************************************************************************************************************
										//Macro definitions
//*************************************************************************************************************/
//#define TASK_ECG_PROCESS_STACK_SIZE			(2048/sizeof(portSTACK_TYPE))
//#define TASK_ECG_PROCESS_PRIORITY			4
//#define TASK_IDLE_STACK_SIZE				(2048/sizeof(portSTACK_TYPE))
//#define TASK_IDLE_PRIORITY					0
//#define TASK_ADS_DATA_STACK_SIZE			(2048/sizeof(portSTACK_TYPE))
//#define TASK_ADS_DATA_PRIORITY				5
//#define TASK_nonin_HR_DATA_STACK_SIZE		(2048/sizeof(portSTACK_TYPE))
//#define TASK_nonin_HR_DATA_PRIORITY			3
//#define SPI_CHSEL							spi_get_pcs(0)
//#define SPI_CHANN							0
//#define ADS_CH1_OFFSET						3
//#define ADS_CH2_OFFSET						6
//#define ADS_CH3_OFFSET						9
////#define DEBUG_MODE 
////#define TESTING_ECG
////#define TESTING_SPO2
////#define NONIN_DATA_TEST
//
///*************************************************************************************************************
										//Function prototypes
//*************************************************************************************************************/
//extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
////extern void vApplicationTickHook(void);
//extern void xPortSysTickHandler(void);
//void spi_init(Spi *p_spi);
//void ioport_config(void);
//void USART0_init(void);
//void USART1_init(void);
//void ADS_DRDY_handler(void);
//void ADS_init(void);
//void read_ADS_config(unsigned char* regs);
//
////Task prototypes
//static void ECG_process_task(void* pvParameters);
//static void idle_task(void* pvParameters);
//static void ADS_data_task(void* pvParameters);
//static void nonin_data_task(void* pvParameters);
//
//
///*************************************************************************************************************
										//Semaphores and queues  
//*************************************************************************************************************/
//xSemaphoreHandle xECG_semaphore; //Semaphore to lock the ECG task while data is ready.
//xSemaphoreHandle xADS_semaphore; //Semaphore to lock the ADS task while DRDY signal is ready.
//xQueueHandle nonin_Queue_Handle = NULL; //SpO2 task queue.
//
///*************************************************************************************************************
										//Variables and constants
//*************************************************************************************************************/
//
////ECG global data
//sensor_state ECG_super_state = SENSOR_ACTIVE_STATE;
//unsigned char ADS_config_regs[0x1A];
//bool ecg_rq = false;
 //
 ////Data buffers
//const signed long mother_wavelet[] = {-1,-2,-4,-8,-16,-26,-37,-44,-41,-22,12,53,87,100,87,53,12,-22,-41,-44,-37,-26,-16,-8,-4,-2,-1};
//signed long sample_buffer[MOTHER_WAVELET_LENGTH];
//signed long threshold_buffer[THRESHOLD_BUFF_SIZE];
//char ECG_data_buffer[15];
//struct ECG_buffer ECG_comm_buffer = {.buffer_index = 0, .full_flag = false};
//uint8_t var_buffer[10];	
//
//#ifdef DEBUG_MODE
//unsigned char dbg_counter = 0;
//#endif
//
//
///**
 //* \brief Handler for Sytem Tick interrupt.
 //*/
//void SysTick_Handler(void)
//{
	//xPortSysTickHandler();
//}
//
//#ifdef NONIN_M2
////SpO2 Data Task (Format #2)
//static void nonin_data_task(void* pvParameters){
	//char SendChar_Alt[8];
	//uint32_t rx_byte;
	//static uint8_t byte_counter = 0;
	//static uint8_t Start = 0;
	//uint8_t currentByte;
	//uint8_t index = 0;
	//
	//for (;;){
		//if( nonin_Queue_Handle != 0 ){
			//if (xQueueReceive(nonin_Queue_Handle, &rx_byte, 1000)){
				//if (nonin_m2_receiver((uint8_t)rx_byte) == NONIN_STRUCT_FILLED){
					//if (get_m2_PrefusionStatus() == GREEN_PERFUSION ){
						//usart_write_line(USART1, "GREEN!");
						//} else if(get_m2_PrefusionStatus() == RED_PERFUSION) {
						//usart_write_line(USART1, "RED!");
						//} else if (get_m2_PrefusionStatus() == YELLOW_PERFUSION){
						//usart_write_line(USART1, "RED!");
					//}
				//}
			//}/* End Queue received */
		//}/* End Queue Created*/
	//}/* End For*/
//}
//#else
////SpO2 Data Task (Format #1)
//static void nonin_data_task(void* pvParameters){
	//uint32_t rx_byte;
	//char SendChar_Alt[8];
				//
	//for (;;){
		//if( nonin_Queue_Handle != 0 ){
			//if (xQueueReceive(nonin_Queue_Handle, &rx_byte, 1000)){
				//if (nonin_m1_receiver((uint8_t)rx_byte) == NONIN_STRUCT_FILLED){
								//
					//sprintf(SendChar_Alt,"        ");
					//sprintf(SendChar_Alt,"HR: %d ",nonin_hrValue);
					//usart_write_line(USART1, &SendChar_Alt);
								//
					//sprintf(SendChar_Alt,"        ");
					//sprintf(SendChar_Alt,"SPo2: %d\n",nonin_Spo2Value);
					//usart_write_line(USART1, &SendChar_Alt);
								//
				//}
			//}/* End Queue received */
		//}/* End Queue Created*/
	//}//End For
//}
//#endif
//
////Background task
//static void idle_task(void* pvParameters){
	//UNUSED(pvParameters);
	////static bool user_led = false;
		//
	//for(;;){
		////usart_putchar(USART1, 0x45);
		//vTaskDelay(1000);
	//}
//}


int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	eic_enable(EIC);
	board_init();
		
	///*Create Queues*/
	//nonin_Queue_Handle = xQueueCreate(200, sizeof(uint8_t));
	//
	///*Create Semaphores*/
	//vSemaphoreCreateBinary(xECG_semaphore);
	//vQueueAddToRegistry(xECG_semaphore, "ECG Semaphore");
	//xSemaphoreTake(xECG_semaphore,0);
	//
	//vSemaphoreCreateBinary(xADS_semaphore);
	//vQueueAddToRegistry(xADS_semaphore, "ADS Semaphore");
	//xSemaphoreTake(xADS_semaphore, 0);
	//
	///* Create Sp02_HR task*/
	//if(xTaskCreate(nonin_data_task, "nonin_HR Data", TASK_nonin_HR_DATA_STACK_SIZE, NULL, TASK_nonin_HR_DATA_PRIORITY, NULL) != pdPASS){
		//printf("Failed to create test SpO2 data task\r\n");
	//}
	///* Create idle task*/
	//if(xTaskCreate(idle_task, "Idle task", TASK_IDLE_STACK_SIZE, NULL, TASK_IDLE_PRIORITY, NULL) != pdPASS){
		////printf("Failed to create test idle task\r\n");
	//}
	///* Start the scheduler. */
	//vTaskStartScheduler();
	///* Will only get here if there was insufficient memory to create the idle task. */
	return 0;
}

/*************************************************************************************************************
							ISRs and callback functions
*************************************************************************************************************/

//
////USART0 handler for SpO2 module comm.
//void USART0_Handler (void){
	//volatile uint32_t status = usart_get_status(USART0);////
	//if(status & US_CSR_RXBUFF){
		//uint32_t rx_byte;
		//usart_read(USART0, &rx_byte);//
		//#ifdef	NONIN_DATA_TEST
			///* TEST */
			//char SendChar_Alt[8];
			//sprintf(SendChar_Alt,"Data: %d\n", rx_byte);
			//usart_write_line(USART1, &SendChar_Alt);
		//#else
			//if (nonin_Queue_Handle != 0){
				//xQueueSend(nonin_Queue_Handle, &rx_byte, 1000);
			//}
		//#endif
	//}	
//}
//
////ADS data ready handler
//void ADS_DRDY_handler(void){
//
//}
//
//
////USART1 handler for radio comm
//void USART1_Handler (void){
//
//}
//
////Timer TC2 handler
//#ifdef DEBUG_MODE
//void TC12_Handler(void){
	//static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
//
	//xSemaphoreGiveFromISR(xADS_semaphore, &xHigherPriorityTaskWoken);
	//portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//
	//if ((tc_get_status(TC1, 2) & TC_SR_COVFS) == TC_SR_COVFS){
		//dbg_counter++;
		//if(dbg_counter == 6){
			//dbg_counter = 0;
			////printf("Counter overflow! \r\n");
		//}
	//}
//}
//#endif