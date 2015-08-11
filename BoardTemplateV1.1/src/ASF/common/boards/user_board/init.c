/**
 * \file
 *
 * \brief User board initialization template
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "ADS/ADS.h"

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)

void board_init(void)
{
	ioport_init();

	//SPI interface initialization
#ifdef CONF_SPI
	//MISO
	ioport_set_pin_peripheral_mode(PIN_PA21, IOPORT_MODE_MUX_A);
	//MOSI
	ioport_set_pin_peripheral_mode(PIN_PA22, IOPORT_MODE_MUX_A);
	//SCK
	ioport_set_pin_peripheral_mode(PIN_PA23, IOPORT_MODE_MUX_A);
	//CS0
	ioport_set_pin_peripheral_mode(PIN_PA24, IOPORT_MODE_MUX_A);
	//CS1
	ioport_set_pin_peripheral_mode(PIN_PA13, IOPORT_MODE_MUX_C);
	//CS2
	ioport_set_pin_peripheral_mode(PIN_PA14, IOPORT_MODE_MUX_C);
	//CS3
	ioport_set_pin_peripheral_mode(PIN_PB12, IOPORT_MODE_MUX_B);

	spi_enable_clock(SPI);
	spi_disable(SPI);
	spi_reset(SPI);
	spi_set_master_mode(SPI);
	spi_disable_mode_fault_detect(SPI);
	spi_disable_loopback(SPI);
	spi_set_variable_peripheral_select(SPI);
	spi_disable_peripheral_select_decode(SPI);
	//spi_set_peripheral_chip_select_value(SPI, SPI_CHSEL);
	//spi_set_transfer_delay(SPI, 1, 50, 0);
	//spi_set_delay_between_chip_select(SPI, 0);

	for(char i = 0; i < 4; i++){
		spi_set_bits_per_transfer(SPI, i, 8);
		//spi_set_baudrate_div(SPI, i, spi_calc_baudrate_div(1000000, sysclk_get_cpu_hz()));
		spi_set_baudrate_div(SPI, i, (sysclk_get_cpu_hz() / 500000));
		spi_configure_cs_behavior(SPI, i, SPI_CS_KEEP_LOW);
		spi_set_clock_polarity(SPI, i, 0);
		spi_set_clock_phase(SPI, i, 0);
	}

	spi_enable(SPI);
#endif

//USART0 initialization
#ifdef CONF_USART0
	//USART0 RXD
	ioport_set_pin_peripheral_mode(PIN_PA11, IOPORT_MODE_MUX_A);

	#if SAM4L
		sysclk_enable_peripheral_clock(USART0);
	#endif
	
	//USART0 configuration struct
	const sam_usart_opt_t usart0_console_settings = {
		CONF_USART_0_BAUDRATE,
		CONF_USART_0_CHAR_LENGTH,
		CONF_USART_0_PARITY,
		CONF_USART_0_STOP_BITS,
		US_MR_CHMODE_NORMAL
	};
	
	usart_init_rs232(USART0, &usart0_console_settings, sysclk_get_main_hz());
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
	usart_enable_interrupt(USART0, US_IER_RXRDY);
	NVIC_SetPriority(USART0_IRQn, 10);
	NVIC_EnableIRQ(USART0_IRQn);
#endif


//USART1 initialization
#ifdef CONF_USART1
	//USART1 TXD
	ioport_set_pin_peripheral_mode(PIN_PA16, IOPORT_MODE_MUX_A);
	//USART1 RXD
	ioport_set_pin_peripheral_mode(PIN_PA15, IOPORT_MODE_MUX_A);
	
	#if SAM4L
		sysclk_enable_peripheral_clock(USART1);
	#endif
	
	//USART1 configuration struct
	const sam_usart_opt_t usart1_console_settings = {
		CONF_USART_1_BAUDRATE,
		CONF_USART_1_CHAR_LENGTH,
		CONF_USART_1_PARITY,
		CONF_USART_1_STOP_BITS,
		US_MR_CHMODE_NORMAL
	};
	
	usart_init_rs232(USART1, &usart1_console_settings, sysclk_get_main_hz());
	usart_enable_tx(USART1);
	usart_enable_rx(USART1);
	usart_enable_interrupt(USART1, US_IER_RXRDY);
	//NVIC_SetPriority(USART0_IRQn, 10);
	NVIC_EnableIRQ(USART1_IRQn);
#endif


#ifdef CONF_TWIMS1
	//SDA
	ioport_set_pin_peripheral_mode(PIN_PB00, IOPORT_MODE_MUX_A);
	//SCL
	ioport_set_pin_peripheral_mode(PIN_PB01, IOPORT_MODE_MUX_A);

	/* Set TWIM options */
	uint32_t cpu_speed = 0;
	cpu_speed = sysclk_get_peripheral_bus_hz(EXAMPLE_TWIM);
	struct twim_config opts = {
		.twim_clk = sysclk_get_cpu_hz(),		//Importante
		.speed = TWIM_MASTER_SPEED,
		.hsmode_speed = 0,
		.data_setup_cycles = 0,
		.hsmode_data_setup_cycles = 0,
		.smbus = false,
		.clock_slew_limit = 0,
		.clock_drive_strength_low = 0,
		.data_slew_limit = 0,
		.data_drive_strength_low = 0,
		.hs_clock_slew_limit = 0,
		.hs_clock_drive_strength_high = 0,
		.hs_clock_drive_strength_low = 0,
		.hs_data_slew_limit = 0,
		.hs_data_drive_strength_low = 0,
	};
	/* Initialize the TWIM Module */
	twim_set_callback(EXAMPLE_TWIM, 0, twim_default_callback, 1);
	twim_set_config(EXAMPLE_TWIM, &opts);
#endif

//ADS 1294R initialization
#ifdef CONF_ADS

	ADS_ioconfig();
		
	Soft_Reset_ADS1298();
	delay_us(50);
	
	Stop_Read_Data_Continuous();
	/*Configuration register 1*/
	ADS1298_SPI_Address_Byte_Count(WRITE_CONFIG_1_REGISTER , SINGLE_BYTE);
	ADS1298_SPI_Data(0x06);
	/*Configuration register 2*/
	ADS1298_SPI_Address_Byte_Count(WRITE_CONFIG_2_REGISTER , SINGLE_BYTE);
	ADS1298_SPI_Data(0x00);
	/*Configuration register 3*/
	ADS1298_SPI_Address_Byte_Count(WRITE_CONFIG_3_REGISTER , SINGLE_BYTE);
	ADS1298_SPI_Data(0xDC);
	/*Channel 1 register*/
	ADS1298_SPI_Address_Byte_Count(WRITE_CHANNEL_1_SET_REGISTER, SINGLE_BYTE);
	ADS1298_SPI_Data(0x00);
	/*Channel 2 register*/
	ADS1298_SPI_Address_Byte_Count(WRITE_CHANNEL_2_SET_REGISTER , SINGLE_BYTE);
	ADS1298_SPI_Data(0x00);
	/*Channel 3 register*/
	ADS1298_SPI_Address_Byte_Count(WRITE_CHANNEL_3_SET_REGISTER , SINGLE_BYTE);
	ADS1298_SPI_Data(0x00);
	/*RLD_SENSP register*/
	ADS1298_SPI_Address_Byte_Count(WRITE_RIGHT_LEG_DRIVE_SENSE_POSITIVE_REGISTER, SINGLE_BYTE);
	ADS1298_SPI_Data(0x0F);
	/*RLD_SENSN register*/
	ADS1298_SPI_Address_Byte_Count(WRITE_RIGHT_LEG_DRIVE_SENSE_NEGATIVE_REGISTER, SINGLE_BYTE);
	ADS1298_SPI_Data(0x0F);
	/*Respiration control register*/
	//Respiration channel not enabled
#endif
}

