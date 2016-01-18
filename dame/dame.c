//*****************************************************************************
//
// dame.c - The game dame.
//
// Rules:
// The pitch is 8x8.
// The stones can move on the black fields.
// The stones can only move diagonal.
// The stones can only move in the direction of the enemy.
// The stones can only move one field at a time.
// If a stone beats another one, the stone can be moved a second, third, etc...
// 		time, until there is no more near stone to beat.
//		(With every move the stone has to beat another stone).
// If a stone can beat another players stone, the player has to beat the stone.
//		No other move is possible.
// If a stone of a player reaches the last row, the kingsrow, it turns into
// 		a king.
//		Kings can move in both directions, up AND down.
//		Kings can only move one field at a time.
// 		Kings behave similiar to regular stones.
// After finishing the move, its the next players turn.
//
//*****************************************************************************

#include <stdio.h>

#include "rit128x96x4.h"
#include "hw_types.h"

#include "graphics.h"
#include "globals.h"
#include "move.h"
#include "computer.h"

unsigned char stoneWhite[50] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xAA,
		0xA0, 0x00, 0x05, 0xAF, 0xFF, 0xFA, 0x00, 0x0A, 0xFF, 0xFF, 0xFF, 0x50,
		0x0A, 0xFF, 0xFF, 0xFF, 0xA0, 0x0A, 0xFF, 0xFF, 0xFF, 0xA0, 0x0A, 0xFF,
		0xFF, 0xFF, 0x50, 0x05, 0xAF, 0xFF, 0xFA, 0x00, 0x00, 0x5A, 0xAA, 0x50,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned char stoneWhiteKing[50] = { 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x5A,
		0xFF, 0xA0, 0x00, 0x05, 0xAe, 0xFF, 0xeA, 0x00, 0x0A, 0xeA, 0xFF, 0xAe,
		0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0A,
		0xeA, 0xFF, 0xAe, 0x50, 0x05, 0xAe, 0xFF, 0xeA, 0x00, 0x00, 0x5A, 0xFF,
		0x50, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00 };

unsigned char stoneBlack[50] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xAA,
		0x50, 0x00, 0x05, 0xA5, 0x00, 0x55, 0x00, 0x0A, 0x50, 0x00, 0x05, 0x50,
		0x05, 0x00, 0x00, 0x00, 0x50, 0x05, 0x00, 0x00, 0x00, 0x50, 0x05, 0x00,
		0x00, 0x00, 0x50, 0x00, 0x50, 0x00, 0x05, 0x00, 0x00, 0x05, 0x00, 0x50,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned char stoneBlackKing[50] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A,
		0x00, 0x50, 0x00, 0x05, 0xA5, 0x00, 0x55, 0x00, 0x0A, 0x50, 0x00, 0x05,
		0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
		0x00, 0x00, 0x00, 0x50, 0x00, 0x50, 0x00, 0x05, 0x00, 0x00, 0x05, 0x00,
		0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int STONEBEATEN = 0;

struct field *selectedStoneField;
struct field *selection;
struct stone *stoneMoved;

struct player *currentPlayer;
struct player players[2];
struct pitch _pitch;

/**
 * The player can select different fields, depending on the current selection.
 * Nothing selected: 	The player can select one of his stones.
 * Stone selected: 		The player can select a free field.
 * 						The player can select another stone.
 * 						The player can deselect the selected stone and the selection-mode
 * 							turns back into nothing selected.
 */
enum selectionMode {
	_stone, _field
} _selectionMode;

/**
 * What button has been pressed.
 */
enum changed {
	nothing, up, down, left, right
} _changed;

/**
 * The selected field, where the players want to move the stones.
 * Draws an interrupted square around a field.
 *
 *  __  __
 * |	  |
 *
 * |__  __|
 *
 */
void drawSelectedField(struct field *_field) {
	int brightness = 15;
	int x = _field->x;
	int y = _field->y;
	int offset = _pitch.offset;
	int offset_70 = (int) (offset * 0.7);
	int offset_20 = (int) (offset * 0.2);

	// top1
	drawLine(x, y, x + offset_20, y, brightness);
	// top2
	drawLine(x + offset_70, y, x + offset - 1, y, brightness);

	// bot1
	drawLine(x, y + offset - 1, x + offset_20, y + offset - 1, brightness);
	// bot2
	drawLine(x + offset_70, y + offset - 1, x + offset - 1, y + offset - 1,
			brightness);

	// left1
	drawLine(x, y, x, y + offset_20, brightness);
	// left2
	drawLine(x, y + offset_70, x, y + offset - 1, brightness);

	// right1
	drawLine(x + offset - 1, y, x + offset - 1, y + offset_20, brightness);
	// right2
	drawLine(x + offset - 1, y + offset_70, x + offset - 1, y + offset - 1,
			brightness);
}

/**
 * The current selected field.
 * Draws a whole sqare around a field.
 */
void drawSelection(struct field *_field) {

	int brightness = 15;

	// Draw outer Border
	// top
	drawLine(_field->x, _field->y, _field->x + _pitch.offset - 1, _field->y,
			brightness);
	// bottom
	drawLine(_field->x, _field->y + _pitch.offset - 1,
			_field->x + _pitch.offset - 1, _field->y + _pitch.offset - 1,
			brightness);
	// left
	drawLine(_field->x, _field->y, _field->x, _field->y + _pitch.offset - 1,
			brightness);
	// right
	drawLine(_field->x + _pitch.offset - 1, _field->y,
			_field->x + _pitch.offset - 1, _field->y + _pitch.offset - 1,
			brightness);

}

/**
 * Function to draw a living stone into a field.
 *
 * *_field: pointer to the field that contains the stone.
 */
void drawStoneInField(struct field *_field) {

	if (_field->_stone->alive) {

		// if the stone is a king, swap the image.
		unsigned char *tempImg =
				_field->_stone->king ?
						_field->_stone->kingImg : _field->_stone->img;
		int i, k;
		// Iterate through the image and draw it into the framebuffer
		for (i = 0; i < STONEHEIGHT; i++) {
			for (k = 0; k < STONEWIDTH / 2; k++) {
				g_pucFrame[((_field->y + i) * 64) + _field->x / 2 + k] =
						*tempImg++;
			}
		}
	}
}

/**
 * Function to draw a stone to a specific position.
 *
 * x: the x-Coordinate.
 * y: the y-Coordinate.
 */
void drawStone(struct stone *_stone, int x, int y) {
	// if the stone is a king, swap the image.
	unsigned char *tempImg = _stone->king ? _stone->kingImg : _stone->img;
	int i, k;
	// Iterate through the image and draw it into the framebuffer
	for (i = 0; i < STONEHEIGHT; i++) {
		for (k = 0; k < STONEWIDTH / 2; k++) {
			g_pucFrame[((y + i) * 64) + x / 2 + k] = *tempImg++;
		}
	}
}

void clearSelectedField(struct field *_field) {
	int i, k;
	// Iterate through the image and draw it into the framebuffer
	for (i = 0; i < _pitch.offset; i++) {
		for (k = 0; k < _pitch.offset / 2; k++) {
			g_pucFrame[((_field->y + i) * 64) + _field->x / 2 + k] = 0x00;
		}
	}

	if (_field->_stone != NULL) {
		drawStoneInField(_field);
	}
}

/**
 * Use the pitch struct to draw the pitch.
 */
void drawPitch(void) {

	int columns, rows, offset, m;

	columns = rows = _pitch.fieldCount;
	offset = _pitch.offset;

	// Draw outer Border
	// top
	drawLine(_pitch.x - 1, _pitch.y - 1, _pitch.x + offset * columns,
			_pitch.y - 1, 10);
	// bottom
	drawLine(_pitch.x - 1, _pitch.y + offset * rows,
			_pitch.x + offset * columns, _pitch.y + offset * rows, 10);
	// left
	drawLine(_pitch.x - 1, _pitch.y - 1, _pitch.x - 1,
			_pitch.y + offset * columns, 10);
	// right
	drawLine(_pitch.x + offset * columns, _pitch.y - 1,
			_pitch.x + offset * columns, _pitch.y + offset * rows, 10);

	struct field *ppitch = _pitch.fields;
	struct field *end = &_pitch.fields[63];

	while (ppitch <= end) {
		if (ppitch->color) {
			// Draw square
			for (m = 0; m < offset; m++) {
				drawLine(ppitch->x, ppitch->y + m, ppitch->x + offset - 1,
						ppitch->y + m, 5);
			}
		}
		ppitch++;
	}

	if (selectedStoneField != NULL)
		drawSelectedField(selectedStoneField);

	if (selection != NULL)
		drawSelection(selection);

	RIT128x96x4ImageDraw(g_pucFrame, 0, 0, 128, 96);
}

/**
 * Initialize the pitch struct.
 */
void initPitch(void) {
	// setup pitch
	int i, k;
	int color = 1;

	_pitch.size = sizeof(_pitch.fields);
	_pitch.offset = 10;
	_pitch.x = displayWidth / 2 - (8 * _pitch.offset) / 2;
	_pitch.y = displayHeight / 2 - (8 * _pitch.offset) / 2;
	_pitch.fieldCount = 8;

	struct field *ppitch = _pitch.fields;

	for (i = 0; i < 8; i++) {
		for (k = 0; k < 8; k++) {
			ppitch->x = _pitch.x + k * _pitch.offset;
			ppitch->y = _pitch.y + i * _pitch.offset;
			ppitch->color = color;
			ppitch->row = i + 1;
			ppitch->col = k + 1;

			// set all field pointer to null.
			ppitch->top = ppitch->topLeft = ppitch->topRight = ppitch->left =
					ppitch->right = ppitch->bottomLeft = ppitch->bottom =
							ppitch->bottomRight = NULL;

			ppitch->top = ppitch - 8;
			ppitch->topLeft = ppitch - 9;
			ppitch->topRight = ppitch - 7;
			ppitch->left = ppitch - 1;
			ppitch->right = ppitch + 1;
			ppitch->bottomRight = ppitch + 9;
			ppitch->bottom = ppitch + 8;
			ppitch->bottomLeft = ppitch + 7;

			// find the fields position and set the field pointers, depending on their position in the pitch.
			if (i == 0 && k == 0) {
				ppitch->top = ppitch->topLeft = ppitch->topRight =
						ppitch->left = ppitch->bottomLeft = NULL;
			} else if (i == 0 && k == 7) {
				ppitch->top = ppitch->topLeft = ppitch->topRight =
						ppitch->right = ppitch->bottomRight = NULL;
			} else if (i == 7 && k == 0) {
				ppitch->topLeft = ppitch->left = ppitch->bottomLeft =
						ppitch->bottom = ppitch->bottomRight = NULL;
			} else if (i == 7 && k == 7) {
				ppitch->topRight = ppitch->right = ppitch->bottomLeft =
						ppitch->bottom = ppitch->bottomRight = NULL;
			} else if (i == 0) {
				ppitch->top = ppitch->topLeft = ppitch->topRight = NULL;
			} else if (i == 7) {
				ppitch->bottomLeft = ppitch->bottom = ppitch->bottomRight =
				NULL;
			} else if (k == 0) {
				ppitch->topLeft = ppitch->left = ppitch->bottomLeft = NULL;

			} else if (k == 7) {
				ppitch->topRight = ppitch->right = ppitch->bottomRight = NULL;
			}

			ppitch++;
			color = !color;
		}
		color = !color;
	}
}

/**
 * Places each players stones on the fields.
 */
void setPlayerStones(struct player *_player) {
	int startRow = _player->position ? 0 : _pitch.fieldCount * 5;
	int endRow =
			_player->position ?
					(_pitch.fieldCount * 3) - 1 : (_pitch.fieldCount * 8) - 1;
	int i;
	struct stone *_stone = _player->stones;

	for (i = startRow; i <= endRow; i++) {
		if (!_pitch.fields[i].color) {
			_stone->_field = &_pitch.fields[i];
			_pitch.fields[i]._stone = _stone++;
			drawStoneInField(&_pitch.fields[i]);
		}
	}
}

/**
 * Initialize the players.
 */
void initPlayers() {

	int j, n;
	int playerColor = 0;
	int playerPosition = 0;

	// setup players
	for (j = 0; j < sizeof(players) / sizeof(players[0]); j++) {
		players[j].color = playerColor;
		players[j].position = playerPosition;

		// setup stones of the player
		for (n = 0;
				n < sizeof(players[j].stones) / sizeof(players[j].stones[0]);
				n++) {
			players[j].stones[n].king = 0;
			players[j].stones[n].alive = 1;
			players[j].stones[n].img = playerColor ? stoneWhite : stoneBlack;
			players[j].stones[n].kingImg =
					playerColor ? stoneWhiteKing : stoneBlackKing;
			players[j].stones[n]._player = &players[j];

		}

		playerPosition = !playerPosition;
		playerColor = !playerColor;

		setPlayerStones(&players[j]);
	}
}

/**
 * Method to draw the current score.
 */
void drawScoreDisplay(unsigned char *img, int x, int y) {
	int i, k;
	// Iterate through the image and draw it into the framebuffer
	for (i = 0; i < STONEHEIGHT; i++) {
		for (k = 0; k < STONEWIDTH / 2; k++) {
			g_pucFrame[((y + i) * 64) + x / 2 + k] = *(img++);
		}
	}
	if (x < 128 / 2) {
		drawLine(x, STONEHEIGHT + 1, x + STONEWIDTH, STONEHEIGHT + 1, 10);
	} else {
		drawLine(x, y - 1, x + STONEWIDTH, y - 1, 10);
	}
}

void upPressed(void) {
	_changed = up;
}

void downPressed(void) {
	_changed = down;
}

void leftPressed(void) {
	_changed = left;
}

void rightPressed(void) {
	_changed = right;
}

/**
 * Checks if the stone is part of the players stones.
 */
int playerOwnsStone(struct player *_player, struct stone *_stone) {

	if (_stone != NULL) {
		struct stone *tmpStone = currentPlayer->stones;

		while (tmpStone <= &currentPlayer->stones[12]) {
			if (tmpStone == _stone)
				return 1;

			tmpStone++;
		}
	}

	return 0;
}

/**
 * Handles the selection of the current player on the pitch.
 * First the player has to select one of his stones.
 * Than the player has to select a possible field.
 *
 * Returns the next possible field.
 */
int isFieldValid(struct field *field) {

	if (_selectionMode == _stone) {
		// only be able to select the stones of the current player.
		if (field->_stone != NULL && field->_stone->_player == currentPlayer)
			if (playerOwnsStone(currentPlayer, field->_stone))
				return 1;

	} else if (_selectionMode == _field) {
		if (field->color)
			return 0;
		// be able to select free fields on the pitch and the current players stones.
		if (field->_stone == NULL)
			return 1;
		if (field->_stone != NULL && field->_stone->_player == currentPlayer)
			if (playerOwnsStone(currentPlayer, field->_stone))
				return 1;
	}

	return 0;
}

struct field * getFirstStoneAlive(struct player *_player) {
	int i = 0;

	for (i = 0; i < sizeof(_player->stones) / sizeof(_player->stones[0]); i++) {
		if (_player->stones[i].alive)
			return _player->stones[i]._field;
	}

	return _player->stones[0]._field;
}

/**
 * Switch the current player, after a player ends his move.
 */
void switchPlayer(void) {

	int i;

	for (i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		if (currentPlayer != &players[i]) {
			currentPlayer = &players[i];
			break;
		}
	}

	if (selectedStoneField) {
		clearSelectedField(selection);
		selectedStoneField = NULL;
	}

	selection = getFirstStoneAlive(currentPlayer);
	drawPitch();
}

/**
 * Function handles the users Input.
 */
void GPIO_Handler(void) {

	switch (_changed) {
	case up:
		moveToTop();
		break;
	case down:
		moveToBottom();
		break;
	case right:

		moveToRight();
		break;
	case left:

		moveToLeft();
		break;
	default:
		break;
		//no valid input,consequently abort the action
		//return 0;
	}

	drawPitch();
	_changed = nothing;
}

/**
 * Function draws the beaten stones to each players scorepoard.
 */
void updateScorboard(void) {
	struct player *p1, *p2;
	int row = 1;
	int count = 0;
	int i;

	// set players
	p1 = currentPlayer;

	for (i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		if (&players[i] != currentPlayer) {
			p2 = &players[i];
			break;
		}
	}

	// top player is p1
	if (!p1->position) {
		p1 = p2;
		p2 = currentPlayer;
	}

	// draw the stones in 4 rows � 3 stones.
	for (i = 0; i < sizeof(p2->stones) / sizeof(p2->stones[0]); i++) {

		// draw score for p1
		if (!p2->stones[i].alive) {
			drawStone(&p2->stones[i], count * 5, row * 13);
			count++;
			if (count == 3) {
				row++;
				count = 0;
			}
		}

	}

	for (i = 0; i < sizeof(p1->stones) / sizeof(p1->stones[0]); i++) {

		// draw score for p2
		//if (p1.)

	}
}

/**
 * Function removes a stone from the pitch.
 */
void beatStone(struct stone *_stone) {
	if (_stone != NULL) {
		_stone->alive = 0;
		clearSelectedField(_stone->_field);
		_stone->_field->_stone = NULL;
		_stone->_field = NULL;
		STONEBEATEN = 1;

		updateScorboard();
	}
}

/**
 * Check if the move of the player is valid.
 * If the player moves over an enemy stone, this stone will be beaten.
 *
 * src: The stone the player wants to move.
 * dst: The field, the player wants to move the stone.
 */
int validMove(struct field *src, struct field *dst) {

// find out if the selected stone lays at the right or left border.

	if (src->_stone->king) {
		// Stone is King -> the stone can move up AND down.
		if ((dst == src->bottomLeft && src->bottomLeft->_stone == NULL)
				|| (dst == src->bottomRight && src->bottomRight->_stone == NULL)
				|| (dst == src->topLeft && src->topLeft->_stone == NULL)
				|| (dst == src->topRight && src->topRight->_stone == NULL)) {
			// Move stone diagonal for one field
			return 1;

		} else {
			if (dst
					== src->bottomLeft->bottomLeft&& src->bottomLeft->bottomLeft->_stone == NULL) {

				// Move stone diagonal for two fields
				// check for enemy stone
				if (!playerOwnsStone(currentPlayer, src->bottomLeft->_stone)) {
					beatStone(src->bottomLeft->_stone);
					return 1;
				}
			} else if (dst
					== src->bottomRight->bottomRight&& src->bottomRight->bottomRight->_stone == NULL) {
				if (!playerOwnsStone(currentPlayer, src->bottomRight->_stone)) {
					beatStone(src->bottomRight->_stone);
					return 1;
				}
			} else if (dst
					== src->topLeft->topLeft&& src->topLeft->topLeft->_stone == NULL) {
				if (!playerOwnsStone(currentPlayer, src->topLeft->_stone)) {

					beatStone(src->topLeft->_stone);
					return 1;
				}
			} else if (dst
					== src->topRight->topRight&& src->topRight->topRight->_stone == NULL) {
				if (!playerOwnsStone(currentPlayer, src->topRight->_stone)) {

					beatStone(src->topRight->_stone);
					return 1;
				}
			}
		}
	} else {
		// Regular stone, can only move up or down, depending on the players position.

		if (currentPlayer->position) {

			if ((dst == src->bottomLeft && src->bottomLeft->_stone == NULL)
					|| (dst == src->bottomRight
							&& src->bottomRight->_stone == NULL)) {
				return 1;
			} else {
				if (dst
						== src->bottomLeft->bottomLeft&& src->bottomLeft->bottomLeft->_stone == NULL) {
					// check if the field between src and dst contains an enemy stone.
					if (!playerOwnsStone(currentPlayer,
							src->bottomLeft->_stone)) {
						beatStone(src->bottomLeft->_stone);
						return 1;
					}
				} else if (dst
						== src->bottomRight->bottomRight&& src->bottomRight->bottomRight->_stone == NULL) {
					// check if the field between src and dst contains an enemy stone.
					if (!playerOwnsStone(currentPlayer,
							src->bottomRight->_stone)) {
						beatStone(src->bottomRight->_stone);
						return 1;
					}
				}
			}
		} else {

			// Player is bottom -> Move direction is from bottom to top.
			if ((dst == src->topLeft && src->topLeft->_stone == NULL)
					|| (dst == src->topRight && src->topRight->_stone == NULL)) {
				return 1;
			} else {
				if (dst == src->topLeft->topLeft) {
					// check if the field between src and dst contains an enemy stone.
					if (!playerOwnsStone(currentPlayer, src->topLeft->_stone)) {

						beatStone(src->topLeft->_stone);
						return 1;
					}
				} else if (dst == src->topRight->topRight) {
					// check if the field between src and dst contains an enemy stone.
					if (!playerOwnsStone(currentPlayer,
							src->topRight->_stone)) {

						beatStone(src->topRight->_stone);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

/**
 * Moves a stone from the source, to the destination.
 */
void moveStone(struct field *src, struct field *dst) {

	dst->_stone = src->_stone;
	dst->_stone->_field = dst;

	src->_stone = NULL;
	clearSelectedField(src);

	selectedStoneField = NULL;

	stoneMoved = dst->_stone;

// if a stone reaches the enemy's last row, make it to a king.
	if ((dst->bottomLeft == NULL || dst->bottomRight == NULL
			|| dst->topLeft == NULL || dst->topRight == NULL)
			&& (dst->top == NULL || dst->bottom == NULL))
		dst->_stone->king = 1;

	drawStoneInField(dst);

}

/**
 * Function checks, if player can beat a stone.
 */
int canBeatStone(struct stone *_stone) {

	int ret = 0;

	if (_stone != NULL) {
		if (_stone->king) {
			// any direction

			if (_stone->_field->bottomLeft->_stone != NULL
					&& !playerOwnsStone(currentPlayer,
							_stone->_field->bottomLeft->_stone) && _stone->_field->bottomLeft->bottomLeft->_stone == NULL) {
				ret += canBeatStone(_stone->_field->bottomLeft->_stone);
				return ret;
			} else if (_stone->_field->bottomRight->_stone != NULL
					&& !playerOwnsStone(currentPlayer,
							_stone->_field->bottomRight->_stone) && _stone->_field->bottomRight->bottomRight->_stone == NULL) {
				ret += canBeatStone(_stone->_field->bottomRight->_stone);
				return ret;
			} else if (_stone->_field->topLeft->_stone != NULL
					&& !playerOwnsStone(currentPlayer,
							_stone->_field->topLeft->_stone) && _stone->_field->topLeft->topLeft->_stone == NULL) {
				return ret + canBeatStone(_stone->_field->topLeft->_stone);
			} else if (_stone->_field->topRight->_stone != NULL
					&& !playerOwnsStone(currentPlayer,
							_stone->_field->topRight->_stone) && _stone->_field->topRight->topRight->_stone == NULL) {
				return ret + canBeatStone(_stone->_field->topRight->_stone);
			}

		} else {
			if (currentPlayer->position) {
				// top
				// Checks if bottomLeft or bottomRight contains an enemy stone
				// and if bottomLeft or bottomRight of this stone is empty ->
				if (_stone->_field->bottomLeft->_stone != NULL
						&& !playerOwnsStone(currentPlayer,
								_stone->_field->bottomLeft->_stone) && _stone->_field->bottomLeft->bottomLeft->_stone == NULL) {
					return 1;
				} else if (_stone->_field->bottomRight->_stone != NULL
						&& !playerOwnsStone(currentPlayer,
								_stone->_field->bottomRight->_stone) && _stone->_field->bottomRight->bottomRight->_stone == NULL) {
					return 1;
				}
			} else {
				// bottom
				// top
				// Checks if bottomLeft or bottomRight contains an enemy stone
				// and if bottomLeft or bottomRight of this stone is empty ->
				if (_stone->_field->topLeft->_stone != NULL
						&& !playerOwnsStone(currentPlayer,
								_stone->_field->topLeft->_stone) && _stone->_field->topLeft->topLeft->_stone == NULL) {
					return 1;
				} else if (_stone->_field->topRight->_stone != NULL
						&& !playerOwnsStone(currentPlayer,
								_stone->_field->topRight->_stone) && _stone->_field->topRight->topRight->_stone == NULL) {
					return 1;
				}
			}
		}
	}

	return ret;
}

/**
 * Handles the select events for the game.
 *
 * If the player selects a Stone and presses select:
 * 	 	If the field is not already selected	and
 * 	 	If the field has a stone:
 *				If other Field with stone is selected:
 *						Remove selection from selectedStone.
 *				Draw an interrupted line arround the field.
 *				Set selectionMode to field, to select more than a stone.
 *
 */
void selectHandler(void) {

	if (_selectionMode == _stone && selection->_stone != NULL
			&& selection->_stone->alive) {
		if (selectedStoneField == NULL) {
			selectedStoneField = selection;
		} else if (selectedStoneField == selection) {
			clearSelectedField(selectedStoneField);
			selectedStoneField = NULL;
		} else {
			clearSelectedField(selectedStoneField);
			selectedStoneField = selection;
		}

	} else if (_selectionMode == _field) {
		if (selectedStoneField == selection) {

// if the player selects a selected stone again,
// remove the selection from the stone.
			selectedStoneField = NULL;
			_selectionMode = _stone;

		} else if (selection->_stone != NULL
				&& playerOwnsStone(currentPlayer, selection->_stone)
				&& selection->_stone->alive) {

// The player selects another one of his stones.
			clearSelectedField(selectedStoneField);
			selectedStoneField = selection;

		} else if (selection->_stone == NULL
				&& validMove(selectedStoneField, selection)) {

// If the player selects a free field, check if the move is valid,
// in relation to the rules of Dame.

			moveStone(selectedStoneField, selection);
			_selectionMode = _stone;
		}
	}

	if (selectedStoneField != NULL)
		_selectionMode = _field;

	if (stoneMoved != NULL && !canBeatStone(stoneMoved))
		selection = NULL;
	else if (stoneMoved != NULL && canBeatStone(stoneMoved))
		selectedStoneField = stoneMoved->_field;

	drawPitch();
}

void startGame(void) {

	RIT128x96x4Clear();

	initPitch();
	initPlayers();

	drawScoreDisplay(stoneWhite, 0, 0);
	drawScoreDisplay(stoneBlack, 127 - STONEWIDTH + 2, 95 - STONEHEIGHT);

	_changed = nothing;
	_selectionMode = _stone;

	currentPlayer = &players[0];
	selectedStoneField = NULL;
	selection = getFirstStoneAlive(currentPlayer);
	stoneMoved = NULL;

	drawPitch();

	while (1) {

		computerStart();

		switch (_changed) {
		case nothing:
			break;
		default:
			GPIO_Handler();
			break;
		}

		if (stoneMoved != NULL && !STONEBEATEN) {
			switchPlayer();
			stoneMoved = NULL;
		} else if (stoneMoved != NULL && STONEBEATEN
				&& !canBeatStone(stoneMoved)) {
			switchPlayer();
			stoneMoved = NULL;
			STONEBEATEN = 0;
		}

//
// See if the select button was pressed.
//
		if (HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)) {
//
// Clear the button press indicator.
//
			HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS) = 0;

			selectHandler();
		}
	}
}
