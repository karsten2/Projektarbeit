/**
 * Initialize the board:
 * 		Set clock
 * 		Init Display
 *
 * Start the tasks.
 *
 */

#include "hw_sysctl.h"
#include "hw_types.h"

#include "sysctl.h"

#include "rit128x96x4.h"

#include "globals.h"

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
	// Initialize the OLED display.
	//
	RIT128x96x4Init(3500000);

	// Start Multitasking
	initTasks();
}
