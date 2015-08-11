/*
    FreeRTOS V6.0.5 - Copyright (C) 2010 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Type definitions. */
#define portCHAR    char
#define portFLOAT   float
#define portDOUBLE  double
#define portLONG    long
#define portSHORT   short
#define portSTACK_TYPE  unsigned portLONG
#define portBASE_TYPE   long

#if(configUSE_16_BIT_TICKS == 1)
	typedef unsigned portSHORT portTickType;
	#define portMAX_DELAY  (portTickType)0xffff
#else
	typedef unsigned portLONG portTickType;
	#define portMAX_DELAY  (portTickType)0xffffffff
#endif

/* Architecture specifics. */
#define portSTACK_GROWTH    (-1)
#define portTICK_RATE_MS    ((portTickType)1000 / configTICK_RATE_HZ)
#define portBYTE_ALIGNMENT  8

/* Scheduler utilities. */
extern void vPortYieldFromISR(void);

#define portYIELD()      vPortYieldFromISR()
#define portEND_SWITCHING_ISR(xSwitchRequired)   if(xSwitchRequired) vPortYieldFromISR()

/*
 * Set basepri to portMAX_SYSCALL_INTERRUPT_PRIORITY without effecting other
 * registers.  r0 is clobbered.
 */
#define portSET_INTERRUPT_MASK()       \
	__asm volatile                  \
	(                            \
		" mov r0, %0         \n"   \
		" msr basepri, r0    \n" \
		::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY):"r0"  \
	)

/*
 * Set basepri back to 0 without effective other registers.
 * r0 is clobbered.
 */
#define portCLEAR_INTERRUPT_MASK()    \
	__asm volatile                   \
	(                           \
		" mov r0, #0          \n"  \
		" msr basepri, r0     \n"  \
		:::"r0"                \
	)

#define portSET_INTERRUPT_MASK_FROM_ISR()     0;portSET_INTERRUPT_MASK()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)  portCLEAR_INTERRUPT_MASK();(void)x

/* Critical section management. */
extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);

#define portDISABLE_INTERRUPTS()   portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()    portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL()       vPortEnterCritical()
#define portEXIT_CRITICAL()        vPortExitCritical()

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) void vFunction(void *pvParameters)
#define portTASK_FUNCTION(vFunction, pvParameters) void vFunction(void *pvParameters)

#define portNOP()

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
