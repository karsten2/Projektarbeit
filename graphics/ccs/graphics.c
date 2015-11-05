#include "globals.h"
#include "inc/hw_types.h"
#include <string.h>

//*****************************************************************************
//
// Clears the local frame buffer.
//
//*****************************************************************************
void clearFrameBuffer(void) {
	memset(&g_pucFrame[0], 0, sizeof(g_pucFrame));
}

//*****************************************************************************
//
// Draws a line in the local frame buffer using Bresenham's line drawing
// algorithm.
//
//*****************************************************************************

extern void drawline();

void drawLine(long lX1, long lY1, long lX2, long lY2, unsigned long ulLevel) {
	long lError, lDeltaX, lDeltaY, lYStep;
	tBoolean bSteep;

	//
	// Determine if the line is steep.  A steep line has more motion in the Y
	// direction than the X direction.
	//
	if (((lY2 > lY1) ? (lY2 - lY1) : (lY1 - lY2))
			> ((lX2 > lX1) ? (lX2 - lX1) : (lX1 - lX2))) {
		bSteep = true;
	} else {
		bSteep = false;
	}

	//
	// If the line is steep, then swap the X and Y coordinates.
	//
	if (bSteep) {
		lError = lX1;
		lX1 = lY1;
		lY1 = lError;
		lError = lX2;
		lX2 = lY2;
		lY2 = lError;
	}

	//
	// If the starting X coordinate is larger than the ending X coordinate,
	// tehn swap the start and end coordinates.
	//
	if (lX1 > lX2) {
		lError = lX1;
		lX1 = lX2;
		lX2 = lError;
		lError = lY1;
		lY1 = lY2;
		lY2 = lError;
	}

	//
	// Compute the difference between the start and end coordinates in each
	// axis.
	//
	lDeltaX = lX2 - lX1;
	lDeltaY = (lY2 > lY1) ? (lY2 - lY1) : (lY1 - lY2);

	//
	// Initialize the error term to negative half the X delta.
	//
	lError = -lDeltaX / 2;

	//
	// Determine the direction to step in the Y axis when required.
	//
	if (lY1 < lY2) {
		lYStep = 1;
	} else {
		lYStep = -1;
	}

	//
	// Loop through all the points along the X axis of the line.
	//
	for (; lX1 <= lX2; lX1++) {
		//
		// See if this is a steep line.
		//
		if (bSteep) {
			//
			// Plot this point of the line, swapping the X and Y coordinates.
			//
			if (lY1 & 1) {
				g_pucFrame[(lX1 * 64) + (lY1 / 2)] = ((g_pucFrame[(lX1 * 64)
						+ (lY1 / 2)] & 0xf0) | (ulLevel & 0xf));
			} else {
				g_pucFrame[(lX1 * 64) + (lY1 / 2)] = ((g_pucFrame[(lX1 * 64)
						+ (lY1 / 2)] & 0x0f) | ((ulLevel & 0xf) << 4));
			}
		} else {
			//
			// Plot this point of the line, using the coordinates as is.
			//
			if (lX1 & 1) {
				g_pucFrame[(lY1 * 64) + (lX1 / 2)] = ((g_pucFrame[(lY1 * 64)
						+ (lX1 / 2)] & 0xf0) | (ulLevel & 0xf));
			} else {
				g_pucFrame[(lY1 * 64) + (lX1 / 2)] = ((g_pucFrame[(lY1 * 64)
						+ (lX1 / 2)] & 0x0f) | ((ulLevel & 0xf) << 4));
			}
		}

		//
		// Increment the error term by the Y delta.
		//
		lError += lDeltaY;

		//
		// See if the error term is now greater than zero.
		//
		if (lError > 0) {
			//
			// Take a step in the Y axis.
			//
			lY1 += lYStep;

			//
			// Decrement the error term by the X delta.
			//
			lError -= lDeltaX;
		}
	}
}
