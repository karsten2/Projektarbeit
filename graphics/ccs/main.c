//*****************************************************************************
//
// graphics.c - Simple Graphics Display Example
//
// Copyright (c) 2006-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 6075 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "drivers/rit128x96x4.h"

#include "globals.h"
#include "random.h"
#include "can_net.h"
#include "game.h"
#include "demo.h"

#include <stdio.h>
#include <string.h>

int menuPointer, menuSize;
const char *menu[5];
unsigned char g_pucFrame[6144];
int displayWidth = 128;
int displayHeight = 96;
int pin0 = 1;
int pin0_released = 0;
int pin1 = 2;
int pin1_released = 0;
int pin2 = 4;
int pin2_released = 0;
int pin3 = 8;
int pin3_released = 0;

//*****************************************************************************
//
// A set of flags used to track the state of the application.
//
//*****************************************************************************
unsigned long g_ulFlags;

//*****************************************************************************
//
// The number of clock ticks that have occurred.  This is used as an entropy
// source for the random number generator; the number of clock ticks at the
// time of a button press or release is an entropic event.
//
//*****************************************************************************
static unsigned long g_ulTickCount = 0;

//*****************************************************************************
//
// The number of clock ticks that have occurred since the last screen update
// was requested.  This is used to divide down the system clock tick to the
// desired screen update rate.
//
//*****************************************************************************
static unsigned char g_ucScreenUpdateCount = 0;

//*****************************************************************************
//
// The number of clock ticks that have occurred since the last application
// update was performed.  This is used to divide down the system clock tick to
// the desired application update rate.
//
//*****************************************************************************
static unsigned char g_ucAppUpdateCount = 0;

//*****************************************************************************
//
// The debounced state of the five push buttons.  The bit positions correspond
// to:
//
//     0 - Up
//     1 - Down
//     2 - Left
//     3 - Right
//     4 - Select
//
//*****************************************************************************
unsigned char g_ucSwitches = 0x1f;

//*****************************************************************************
//
// The vertical counter used to debounce the push buttons.  The bit positions
// are the same as g_ucSwitches.
//
//*****************************************************************************
static unsigned char g_ucSwitchClockA = 0;
static unsigned char g_ucSwitchClockB = 0;

//*****************************************************************************
//
// The speed of the processor clock, which is therefore the speed of the clock
// that is fed to the peripherals.
//
//*****************************************************************************
unsigned long g_ulSystemClock;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

void initMenu(void) {
	menu[0] = "Button Demo";
	menu[1] = "Draw a Line";
	menu[2] = "Move the Dot";
	menu[3] = "Draw Image";
	menu[4] = "Start Game";
	menuSize = 5;
	menuPointer = 0;
}

void drawMenu(void) {

	RIT128x96x4Clear();
	int i;
	char entry[80];
	for (i = 0; i < menuSize; i++) {
		if (i == menuPointer) {
			strcpy(entry, menu[i]);
			strcat(entry, "<---");
		} else {
			strcpy(entry, menu[i]);
		}
		RIT128x96x4StringDraw(entry, 0, i * 10, 15);
	}
}

//*****************************************************************************
//
// Delay for a multiple of the system tick clock rate.
//
//*****************************************************************************
static void Delay(unsigned long ulCount) {
	//
	// Loop while there are more clock ticks to wait for.
	//
	while (ulCount--) {
		//
		// Wait until a SysTick interrupt has occurred.
		//
		while (!HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK)) {
		}

		//
		// Clear the SysTick interrupt flag.
		//
		HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 0;
	}
}

// up
void GPIO_0_Released(void) {
	if (menuPointer > 0) {
		menuPointer--;
		drawMenu();
	}
}

// down
void GPIO_1_Released(void) {
	if (menuPointer < menuSize - 1) {
		menuPointer++;
		drawMenu();
	}
}

// left
void GPIO_2_Released(void) {
	// do something
}

// right
void GPIO_3_Released(void) {
	// do something

}

void GPIO_Pin_Handler(void) {
	int pin0_newState = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0);
	int pin1_newState = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
	int pin2_newState = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2);
	int pin3_newState = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);

	// Pin 0
	if (pin0 != pin0_newState) {
		if (pin0 == 1 && pin0_newState == 0) {
			// button Pressed
			pin0 = 0;
		} else if (pin0 == 0 && pin0_newState == 1) {
			// button released
			pin0 = 1;
			pin0_released = 1;
		}
	}
	// Pin 1
	if (pin1 != pin1_newState) {
		if (pin1 == 2 && pin1_newState == 0) {
			// button Pressed
			pin1 = 0;
		} else if (pin1 == 0 && pin1_newState == 2) {
			// button released
			pin1 = 2;
			pin1_released = 1;
		}
	}
	// Pin 2
	if (pin2 != pin2_newState) {
		if (pin2 == 4 && pin2_newState == 0) {
			// button Pressed
			pin2 = 0;
		} else if (pin2 == 0 && pin2_newState == 4) {
			// button released
			pin2 = 4;
			pin2_released = 1;
		}
	}
	// Pin 3
	if (pin3 != pin3_newState) {
		if (pin3 == 8 && pin3_newState == 0) {
			// button Pressed
			pin3 = 0;
		} else if (pin3 == 0 && pin3_newState == 8) {
			// button released
			pin3 = 8;
			pin3_released = 1;
		}
	}
}

//*****************************************************************************
//
// Handles the SysTick timeout interrupt.
//
//*****************************************************************************
void SysTickIntHandler(void) {
	unsigned long ulData, ulDelta;

	//
	// Increment the tick count.
	//
	g_ulTickCount++;

	//
	// Indicate that a timer interrupt has occurred.
	//
	HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 1;

	//
	// Increment the screen update count.
	//
	g_ucScreenUpdateCount++;

	//
	// See if 1/30th of a second has passed since the last screen update.
	//
	if (g_ucScreenUpdateCount == (CLOCK_RATE / 30)) {
		//
		// Restart the screen update count.
		//
		g_ucScreenUpdateCount = 0;

		//
		// Request a screen update.
		//
		HWREGBITW(&g_ulFlags, FLAG_UPDATE) = 1;
	}

	//
	// Increment the application update count.
	//
	g_ucAppUpdateCount++;

	//
	// See if 1/100th of a second has passed since the last application update.
	//
	if (g_ucAppUpdateCount != (CLOCK_RATE / 100)) {
		//
		// Return without doing any further processing.
		//
		return;
	}

	//
	// Restart the application update count.
	//
	g_ucAppUpdateCount = 0;

	//
	// Read the state of the push buttons.
	//
	ulData =
			(GPIOPinRead(GPIO_PORTE_BASE, (GPIO_PIN_0 | GPIO_PIN_1 |
			GPIO_PIN_2 | GPIO_PIN_3))
					| (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) << 3));

	//
	// Determine the switches that are at a different state than the debounced
	// state.
	//
	ulDelta = ulData ^ g_ucSwitches;

	//
	// Increment the clocks by one.
	//
	g_ucSwitchClockA ^= g_ucSwitchClockB;
	g_ucSwitchClockB = ~g_ucSwitchClockB;

	//
	// Reset the clocks corresponding to switches that have not changed state.
	//
	g_ucSwitchClockA &= ulDelta;
	g_ucSwitchClockB &= ulDelta;

	//
	// Get the new debounced switch state.
	//
	g_ucSwitches &= g_ucSwitchClockA | g_ucSwitchClockB;
	g_ucSwitches |= (~(g_ucSwitchClockA | g_ucSwitchClockB)) & ulData;

	//
	// Determine the switches that just changed debounced state.
	//
	ulDelta ^= (g_ucSwitchClockA | g_ucSwitchClockB);

	//
	// See if any switches just changed debounced state.
	//
	if (ulDelta) {
		GPIO_Pin_Handler();

		//
		// Add the current tick count to the entropy pool.
		//
		RandomAddEntropy(g_ulTickCount);
	}

	//
	// See if the select button was just pressed.
	//
	if ((ulDelta & 0x10) && !(g_ucSwitches & 0x10)) {
		//
		// Set a flag to indicate that the select button was just pressed.
		//
		HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS) = 1;
	}
}

int main(void) {

	//
	// If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
	// a workaround to allow the PLL to operate reliably.
	//
	if (REVISION_IS_A2) {
		SysCtlLDOSet(SYSCTL_LDO_2_75V);
	}

	//
	// Set the clocking to run directly from the crystal.
	//
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_8MHZ);

	//
	// Get the system clock speed.
	//
	g_ulSystemClock = SysCtlClockGet();

	//
	// Initialize the OLED display.
	//
	RIT128x96x4Init(3500000);

	//
	// Get the system clock speed.
	//
	g_ulSystemClock = SysCtlClockGet();

	//
	// Enable Peripherals
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//
	// Configure the GPIOs used to read the state of the on-board push buttons.
	//
	// Port E up, down, left, right
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
	GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTE_BASE,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
			GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD_WPU);

	// Port F select
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
	GPIO_PIN_TYPE_STD_WPU);

	//
	// Initialize the CAN controller.
	//
	CANConfigure();

	//
	// Configure SysTick to periodically interrupt.
	//
	SysTickPeriodSet(g_ulSystemClock / CLOCK_RATE);
	SysTickIntEnable();
	SysTickEnable();

	//
	// Delay for a bit to allow the initial display flash to subside.
	//
	Delay(CLOCK_RATE / 4);

	initMenu();
	drawMenu();

	startGame();

	while (1) {

		if (pin0_released) {
			GPIO_0_Released();
			pin0_released = 0;
		}
		if (pin1_released) {
			GPIO_1_Released();
			pin1_released = 0;
		}
		if (pin2_released) {
			GPIO_2_Released();
			pin2_released = 0;
		}
		if (pin3_released) {
			GPIO_3_Released();
			pin3_released = 0;
		}

		//
		// See if the select button was pressed.
		//
		if (HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)) {
			//
			// Clear the button press indicator.
			//
			HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS) = 0;

			switch (menuPointer) {
			case 0:
				// Button Demo
				//buttonDemo();
				break;
			case 1:
				// Draws a Line
				//lineDemo();
				break;
			case 2:
				// Move a dot
				//dotDemo();
				break;
			case 3:
				// Draw Image
				drawImage();
				break;
			case 4:
				// Game
				startGame();
				break;
			}
			drawMenu();
		}
	}
}
