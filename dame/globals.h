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
// Function prototypes
//
//*****************************************************************************
extern void upPressed();
extern void downPressed();
extern void leftPressed();
extern void rightPressed();

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
} stone;

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
	int col, row;
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

//*****************************************************************************
//
// The set of switches that are currently pressed.
//
//*****************************************************************************
extern unsigned char g_ucSwitches;

extern player * startGame();
extern void initTasks();
extern void computerStart();

#endif // __GLOBALS_H__
