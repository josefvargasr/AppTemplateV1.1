/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <sam4lc4b.h>
#include <conf_board.h>
#include <flashcalw.h>

// Oscillator Hardware Configurations

#define FOSC0                     (12000000UL)
// Osc32 frequency (Hz.) and startup time (RCOsc periods)
#define FOSC32                    (32768UL)
#define BOARD_OSC32_IS_XTAL       true
#define BOARD_OSC32_HZ            FOSC32
#define BOARD_OSC32_STARTUP_US    (750000UL)
#define BOARD_OSC32_SELCURR       BSCIF_OSCCTRL32_SELCURR(10)
#define BOARD_OSC0_IS_XTAL        true
#define BOARD_OSC0_HZ             FOSC0
#define BOARD_OSC0_STARTUP_US     (6100UL)

/** \name TWIM definitions */
#define EXAMPLE_TWIM_IRQn    TWIM1_IRQn				// TWIM Interrupt Number
#define EXAMPLE_TWIM         TWIM1					// TWIM Module Used
#define TWIM_MASTER_SPEED    TWI_STD_MODE_SPEED		// Speed of TWI
#define EXAMPLE_TWIM_Handler TWIM1_Handler			// TWIM Interrupt Handler

#define SPI_MISO				PIN_PA21
#define SPI_MOSI				PIN_PA22
#define SPI_SCK					PIN_PA23
#define SPI_CS0     			PIN_PA24
#define SPI_CS1     			PIN_PA13
#define SPI_CS2     			PIN_PA14
#define SPI_CS3     			PIN_PB12

#define USART0_RXD  			PIN_PA11

#define USART1_TXD  			PIN_PA16
#define USART1_RXD  			PIN_PA15

#define TWIMS1_SDA  			PIN_PB00
#define TWIMS1_SCL  			PIN_PB01

#define PORT_ADS_DRDY           PIN_PA20
#define PORT_ADS_CS             SPI_CS1
#define PORT_ADS_CLKSEL         PIN_PA18
#define PORT_ADS_START          PIN_PA10
#define PORT_ADS_POWER          PIN_PA17
#define PORT_ADS_RESET          PIN_PA12

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000


#endif // USER_BOARD_H
