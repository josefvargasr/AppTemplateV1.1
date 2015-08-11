/*
 * ADS.c
 *
 * Created: 08/10/2014 14:55:22
 *  Author: Jose Arboleda
 */ 

#define RELEASE 

#ifdef RELEASE

#include "ADS.h"
#include "spi.h"
#include "ioport.h"
#include "eic.h"
#include "delay.h"



extern void ADS_DRDY_handler(void);

void ADS1298_SPI_Address_Byte_Count (unsigned char Read_Write_Address, unsigned char Number_Of_Bytes)
{ 
    unsigned char r_data;
	unsigned char r_pcs;
	
	Clear_ADS1298_Chip_Enable ();
  
    Set_ADS1298_Chip_Enable ();         // Enable CS to SPI transmission

    //UCB0IFG &= ~UCRXIFG;                // Clear reception flag
    //UNUSED(spi_read_status(SPI));
	
    //TX_BUF = Read_Write_Address;        // Transmit the address register where it's gonna be written or read
     spi_write(SPI, Read_Write_Address, spi_get_pcs(ADS_CS_VAL),  0); 
	 
    //while (!( UCB0IFG & UCRXIFG ));     // wait until the buffer is empty
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	delay_cycles((unsigned long)(DELAY_TCKS_4));

    //UCB0IFG &= ~UCTXIFG;                // Clear transmission flag
    //UCB0IFG &= ~UCRXIFG;                // Clear reception flag

    //TX_BUF = Number_Of_Bytes;           // Transmit the number of bytes that it's gonna be written or read
    spi_write(SPI, Number_Of_Bytes, spi_get_pcs(ADS_CS_VAL),  0);
	
    //while (!( UCB0IFG & UCRXIFG ));     // wait until the buffer is empty
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);
	delay_cycles((unsigned long)(DELAY_TCKS_4));
    //UCB0IFG &= ~UCTXIFG;                // Clear transmission flag
    //UCB0IFG &= ~UCRXIFG;                // Clear reception flag
}




unsigned char ADS1298_SPI_Data (unsigned char Data)  
{
    
	unsigned short r_data;
	unsigned char r_pcs;
	
    //UCB0IFG &= ~UCRXIFG;                  //Clear reception flag
    //UNUSED(spi_read_status(SPI));
	
    //TX_BUF = Data;                       //Send the data to the transmission buffer.
    spi_write(SPI, Data, spi_get_pcs(ADS_CS_VAL),  0); 
	//while (!( UCB0IFG & UCRXIFG ));
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);

    //UCB0IFG &= ~UCTXIFG;                  // Clear transmission flag
    //UCB0IFG &= ~UCRXIFG;                  // Clear reception flag
    
    //TODO: Adjust delay for SAM4L
	//unsigned char i;
    //for (i = 50; i > 0; i--);            // Delay
	delay_cycles((unsigned long)(DELAY_TCKS_4));  
    
    
    //return RX_BUF;                       // Return the data read.
	return r_data;
}


unsigned char ADS1298_SPI_Channel_Data (unsigned char Dummy_Data) 
{                                                                               
    unsigned short r_data;
	unsigned char r_pcs;
	
	//UCB0IFG &= ~UCRXIFG;                 // Clear reception flag
	UNUSED(spi_read_status(SPI));
	
    //TX_BUF = Dummy_Data;                 // Send the 8 bits data to the buffer
	spi_write(SPI, Dummy_Data, spi_get_pcs(ADS_CS_VAL),  0);

	// while (!( UCB0IFG & UCRXIFG )) ;
	while((spi_read_status(SPI) & SPI_SR_RDRF) == 0);
	spi_read(SPI, (uint16_t*) &r_data, (uint8_t*) &r_pcs);

    //UCB0IFG &= ~UCTXIFG;                 // Clear transmission flag
    //UCB0IFG &= ~UCRXIFG;                 // Clear reception flag

    //return RX_BUF;                       // Return the data read.
	return r_data;
} 

/**************************************************************************************************************
                                              Functions                                                                      
*************************************************************************************************************/

void Set_ADS1298_Chip_Enable (void)                        // ADS1298 module uses GPIO as the SPI CS (Activo en bajo)
{
    //unsigned char i;
    //SetBit(PORT_ADS_CS_DIR, ADS_CS);                        /* Port 4.3 used as CS */
    //ResetBit(PORT_ADS_CS_OUT, ADS_CS);                      // Active low, 0 to select ADS
    //for (i = 50; i > 0; i--);                             	// Short Delay before invoking the SPI Port
}

void Clear_ADS1298_Chip_Enable (void)                      	// ADS1298 uses GPIO for SPI CS
{
    //unsigned char i;
    //SetBit(PORT_ADS_CS_OUT, ADS_CS);                        // Active low, 1 to diselect ADS 
    //SetBit(PORT_ADS_CS_DIR, ADS_CS);                       	//Port 4.3 used as CS
    //for (i = 50; i > 0; i--);     							//set the pin and MSP430F5438A port whom it's gonna be used as a CS
}

void set_ADS1298_Start_High (void)							// Send 1 to START pin of ADS1294R on order to begin the conversions.
{
  //SetBit(PORT_ADS_START_DIR, START);   
  //SetBit(PORT_ADS_START_OUT, START);
  ioport_set_pin_level(PORT_ADS_START, true);                   	//set the pin and MSP430F5438A port whom it's gonna be used as a START
}

void set_ADS1298_Start_Low (void)							// Send 0 to START pin of ADS1294R on order to stop the conversions.
{
  //SetBit(PORT_ADS_START_DIR, START);   
  //ResetBit(PORT_ADS_START_OUT, START);                     	//set the pin and MSP430F5438A port whom it's gonna be used as a START
  ioport_set_pin_level(PORT_ADS_START, false);
}


void set_ADS1298_Power_Up (void)							// Send 1 to PWDN(power down) pin wich is active low, so send 0                                                             
{															// turns off the ADS1294R.
  //1 means to power up ADS
  //SetBit(PORT_ADS_POWER_DIR, POWER_UP);   
  //SetBit(PORT_ADS_POWER_OUT, POWER_UP);                    	//set the pin and MSP430F5438A port whom it's gonna be used as a PWDN
  ioport_set_pin_level(PORT_ADS_POWER, true);
}

void set_ADS1298_Power_Down (void)							// Send 0 to the PWDN pin of the ADS1294R
{
  //0 means to power down ADS
  //SetBit(PORT_ADS_POWER_DIR, POWER_UP);   
  //ResetBit(PORT_ADS_POWER_OUT, POWER_UP);                   //set the pin and MSP430F5438A port whom it's gonna be used as a PWDN 
  ioport_set_pin_level(PORT_ADS_POWER, false);
}

void set_ADS1298_Clock_Internal (void)						// Send 1 to CLKSEL pin of the ADS1294R
{
  //1 means using internal, power save
  //SetBit(PORT_ADS_CLKSEL_DIR, INTERNAL_CLOCK);   
  //SetBit(PORT_ADS_CLKSEL_OUT, INTERNAL_CLOCK);              //set the pin and MSP430F5438A port whom it's gonna be used as a CLKSEL      
  ioport_set_pin_level(PORT_ADS_CLKSEL, true);
}

void set_ADS1298_Clock_External (void)						// Send 0 to CLKSEL pin of the ADS1294R
{
  //0 means using external
  //SetBit(PORT_ADS_CLKSEL_DIR, INTERNAL_CLOCK);   
  //ResetBit(PORT_ADS_CLKSEL_OUT, INTERNAL_CLOCK);           //set the pin and MSP430F5438A port whom it's gonna be used as a CLKSEL          
  ioport_set_pin_level(PORT_ADS_CLKSEL, false);
}

void Power_On_Reset_ADS1298 (void)							// Reset when the ADS1294R is on in order to ensure all registers                                                       
{															// and channel are set by default.
    unsigned char t;
    
      for (t = 0x20; t > 0; t--);             				// short delay                   
      //SetBit(PORT_ADS_RESET_OUT, ADS_RESET); 
	  ioport_set_pin_level(PORT_ADS_RESET, true);       
      /*for (i = 0xFFFF; i > 0; i--)
        {
            for (w = 0x09; w > 0; w--);       // long Delay
        }*/
      delay_ms(1000);                                    // 1 second Delay for initialization.  
	  //ResetBit(PORT_ADS_RESET_OUT, ADS_RESET);  // Reset Low
      ioport_set_pin_level(PORT_ADS_RESET, false);
	  for (t = 0x10; t > 0; t--);             // short Delay  
      //SetBit(PORT_ADS_RESET_OUT, ADS_RESET);    // Reset High
      ioport_set_pin_level(PORT_ADS_RESET, true);
	  for (t = 0x90; t > 0; t--);             // around 18 Tclk       
}

void Hard_Reset_ADS1298_Off (void)			  // Turn off reset through Hardware.
{
  //1 means NOT reset
  //SetBit(PORT_ADS_RESET_DIR, ADS_RESET);   
  //SetBit(PORT_ADS_RESET_OUT, ADS_RESET);      // Pin of MSP430F5438A used for Reset
  ioport_set_pin_level(PORT_ADS_RESET, true);               
}

void Hard_Reset_ADS1298_On (void)			  // Turn on reset through Hardware
{
  //0 means  reset
  //SetBit(PORT_ADS_RESET_DIR, ADS_RESET);   
  //ResetBit(PORT_ADS_RESET_OUT, ADS_RESET);    //Pin of MSP430F5438 used for Reset     
  ioport_set_pin_level(PORT_ADS_RESET, false);
}

/**************************************************************************************************************************************************
*  Group of functions that configure ans initialize the ADS1294R through software (SPI Opcodes).                                                                                                                                               *
*                                                                                                                                                 *
**************************************************************************************************************************************************/

void Wake_Up_ADS1298 (void)
{
    Set_ADS1298_Chip_Enable (); 
	
    ADS1298_SPI_Data (WAKE_CONVERTER_FROM_SLEEP);                  // Send 0x02 to the ADS1298                                                      
}

void Put_ADS1298_In_Sleep (void)
{
    Set_ADS1298_Chip_Enable (); 
	 
    ADS1298_SPI_Data (PLACE_CONVERTER_IN_SLEEP_MODE);              // Send 0x04 to the ADS1298
}

void Soft_Reset_ADS1298 (void)
{
    //Set_ADS1298_Chip_Enable (); 
	 
    ADS1298_SPI_Data (RESET_CONVERTER);                            // Send 0x06 to the ADS1298
}

void Soft_Start_ReStart_ADS1298 (void)
{
  //Start or restart conversion
    unsigned char i;
  	
    set_ADS1298_Start_Low ();                                                 // Start Pin LOW   
    for (i = 200; i > 0; i--);                                                  // Small Delay   
    ADS1298_SPI_Data (START_RESTART_CONVERSION);                   // Send 0x08 to the ADS1298
    Clear_ADS1298_Chip_Enable ();                                                       
}

void Soft_Stop_ADS1298 (void)
{
    //Stop conversion
    unsigned char i;
  	
    set_ADS1298_Start_Low ();                                                 // Start Pin LOW   
    for (i = 200; i > 0; i--);                                                  // Small Delay   
    Set_ADS1298_Chip_Enable (); 
    ADS1298_SPI_Data (STOP_CONVERSION);                            // Send 0x0A to the ADS1298
}

void Stop_Read_Data_Continuous (void)								// Stop data read continious.Detiene la lectura de datos en modo continuo
{
    Set_ADS1298_Chip_Enable (); 									// Enable the ADS1294R to establish the SPI communication.
	
    ADS1298_SPI_Data (STOP_READ_DATA_CONTINUOUSLY);					// Send the OPCODE 0x11 in order to stop the data read continuous.
}

void Start_Read_Data_Continuous (void)								// Start the data read continuous.
{
    Set_ADS1298_Chip_Enable (); 									// Enable the ADS1294R to establish the SPI communication.
	
    ADS1298_SPI_Data (SET_READ_DATA_CONTINUOUSLY);					// Send the OPCODE 0x11 in order to stop the data read continuous.
}


//SAM4L EIC must be enabled first
void enable_ADS1298_Interrupt (void)		// Enable a pin of interruptions of the MSP430F5438A  
{											// in order to detect the DRDY (Data ready) signal of the ADS1294R.
  
  //P2DIR |= 0x01;                          // Set P1.0 to output direction
  /*P2REN |= 0x20;                            // Enable P1.4 internal resistance
  P2OUT |= 0x20;                            // Set P1.4 as pull-Up resistance
  P2IE |= 0x20;                             // P1.4 interrupt enabled
  P2IES |= 0x20;                            // P1.4 Hi/Lo edge
  P2IFG &= ~0x20;*/                           // P1.4 IFG cleared
  
  struct eic_line_config eic_line_conf;
  eic_line_conf.eic_mode = EIC_MODE_EDGE_TRIGGERED;
  eic_line_conf.eic_edge = EIC_EDGE_FALLING_EDGE;
  eic_line_conf.eic_level = EIC_LEVEL_LOW_LEVEL;
  eic_line_conf.eic_filter = EIC_FILTER_DISABLED;
  eic_line_conf.eic_async = EIC_ASYNCH_MODE;
  eic_line_set_config(EIC,  DRDY_EIC_LINE, &eic_line_conf);
  eic_line_set_callback(EIC,  DRDY_EIC_LINE, ADS_DRDY_handler, DRDY_IRQ, 10);
  eic_line_enable(EIC,  DRDY_EIC_LINE); 
}

void disable_ADS1298_Interrupt (void)		// Disable the P1.4 pin interruption.
{
	//ResetBit(PORT_INT_IE,ADS_DRDY); 
	//P2IE &= ~0x20;
	eic_line_disable_interrupt(EIC,  DRDY_EIC_LINE);
	
}

void enable_ADS1298_Conversion (void)								// Enable the ADS1294R conversions.
{    
    enable_ADS1298_Interrupt ();                                    // Interrupts must be enabled to transmit and receive data
    
    Start_Read_Data_Continuous ();                                  // Enable continuous conversion mode
    
    set_ADS1298_Start_High ();                                   	// Pull START pin high
    
}

unsigned char ADS1298_Read_Version (void)  							// Read Device ID, Single Byte the Part Number
{
    unsigned char Version_ID = 18;
    ADS1298_SPI_Address_Byte_Count (READ_DEVICE_ID, SINGLE_BYTE);    		
    //__delay_cycles(1000);
	//for(int i = 0; i < 50; i++);
    Version_ID = ADS1298_SPI_Data (DUMMY_DATA); 
	//ADS1298_SPI_Data(DUMMY_DATA);                    // Send Dummy variable (0x55) to return the part number (Chip Select Cleared automatically)
	spi_set_peripheral_chip_select_value(SPI, spi_get_pcs(ADS_CS_VAL));
    return Version_ID;
} 


//SAM4L ioport service initialization must be called first
void ADS_ioconfig(void){
	
	//!RESET=1:Activo en Bajo
	ioport_set_pin_dir(PORT_ADS_RESET, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PORT_ADS_RESET, true);
	
	//START=0: Activo en Alto
	ioport_set_pin_dir(PORT_ADS_START, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PORT_ADS_START, false);
	
	//CLKSEL=1: Internal Clock
	ioport_set_pin_dir(PORT_ADS_CLKSEL, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PORT_ADS_CLKSEL, true);
	delay_us(50);
	
	//!PWDN=1: Activo en Bajo
	ioport_set_pin_dir(PORT_ADS_POWER, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PORT_ADS_POWER, true);
	
	//!DRDY interrupt Enable	
	ioport_set_pin_mode(PORT_ADS_DRDY, IOPORT_MODE_MUX_C);
	ioport_disable_pin(PORT_ADS_DRDY);
}

#endif