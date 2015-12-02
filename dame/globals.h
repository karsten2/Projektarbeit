//*****************************************************************************
//
// globals.h - Shared configuration and global variables.
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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

//*****************************************************************************
//
// The clock rate for the SysTick interrupt.  All events in the application
// occur at some fraction of this clock rate.
//
//*****************************************************************************
#define CLOCK_RATE              300

//*****************************************************************************
//
// A set of flags used to track the state of the application.
//
//*****************************************************************************
extern unsigned long g_ulFlags;
#define FLAG_CLOCK_TICK         0           // A timer interrupt has occurred
#define FLAG_CLOCK_COUNT_LOW    1           // The low bit of the clock count
#define FLAG_CLOCK_COUNT_HIGH   2           // The high bit of the clock count
#define FLAG_UPDATE             3           // The display should be updated
#define FLAG_BUTTON             4           // Debounced state of the button
#define FLAG_DEBOUNCE_LOW       5           // Low bit of the debounce clock
#define FLAG_DEBOUNCE_HIGH      6           // High bit of the debounce clock
#define FLAG_BUTTON_PRESS       7           // The button was just pressed
#define FLAG_ENET_RXPKT         8           // An Ethernet Packet received
#define FLAG_ENET_TXPKT         9           // An Ethernet Packet transmitted

//*****************************************************************************
//
// The speed of the processor.
//
//*****************************************************************************
extern unsigned long g_ulSystemClock;

//*****************************************************************************
//
// Storage for a local frame buffer.
//
//*****************************************************************************
extern unsigned char g_pucFrame[6144];
extern int buttonPressed;

extern void upPressed();
extern void downPressed();
extern void leftPressed();
extern void rightPressed();
extern void startGame();

#define STONEWIDTH 		10
#define STONEHEIGHT 	10
#define displayWidth	128
#define displayHeight	96

/**
 * king: [0, 1]: when stone reaches enemies kings row, it will be transformed to a king.
 * alive: [0, 1]: 0 - dead, 1 - alive.
 * *img: Pointer to the image.
 * *kingImg: Pointer to the image, if the stone turns into a king.
 * *_field: The field, that contains the stone.
 * *_player: The Owner of the stone.
 */
typedef struct stone {
	int king;
	int alive;
	unsigned char *img;
	unsigned char *kingImg;
	struct field *_field;
	struct player *_player;
} stone, *stonePtr;

/**
 * A field in the pitch.
 *
 * x: [0-127]: The horizontal position of the field.
 * y: [0-95]: The vertical position of the field.
 * color: [0-1]: The Color of the field.
 * selected: [0-1]:
 * *_stone: a possible stone on the field.
 */
typedef struct field {
	int x, y;
	int color;
	struct stone *_stone;
	struct field *top;
	struct field *topLeft;
	struct field *topRight;
	struct field *left;
	struct field *right;
	struct field *bottom;
	struct field *bottomLeft;
	struct field *bottomRight;
} field;

/**
 * x: [0-127]: The horizontal position of the pitch.
 * y: [0-95]: The vertical position of the pitch.
 * offset: The widht and height of a field. !!! Use only even numbers !!!
 * size: Size of the array.
 * fields: 8x8 array of fields.
 */
typedef struct pitch {
	int x, y;
	int offset;
	int size;
	int fieldCount;
	struct field fields[64];
} pitch;

/**
 * position: [1, 0]: top, bottom
 * color: [0, 1]: 0 - black, 1 - white.
 * stones: The array of the players stones.
 */
typedef struct player {
	int position;
	int color;
	struct stone stones[12];
} player;

extern struct field *selectedStoneField;
extern struct field *selection;
extern struct stone *stoneMoved;

extern struct player *currentPlayer;
extern struct player players[2];
extern struct pitch _pitch;

extern int playerOwnsStone();


//*****************************************************************************
//
// The set of switches that are currently pressed.
//
//*****************************************************************************
extern unsigned char g_ucSwitches;

#endif // __GLOBALS_H__
