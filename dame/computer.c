#include "globals.h"
#include "dynStone.h"
#include <stdio.h>

dynStoneArray movableStones;
dynStoneArray beatableStones;

/**
 * Function checks, if player can move a stone.
 */
int movePossible(struct stone *_stone) {
	if (_stone != NULL) {
		if (_stone->king) {
			if (_stone->_field->bottomLeft->_stone == NULL
					|| _stone->_field->bottomRight->_stone == NULL
					|| _stone->_field->topLeft->_stone == NULL
					|| _stone->_field->topRight->_stone == NULL) {
				return 1;
			}
		}
	} else {
		if (currentPlayer->position) {
			if (_stone->_field->bottomLeft->_stone == NULL
					|| _stone->_field->bottomRight->_stone == NULL) {
				return 1;
			}
		} else {
			if (_stone->_field->topLeft->_stone == NULL
					|| _stone->_field->topRight->_stone == NULL) {
				return 1;
			}
		}
	}
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
 * Find all Stones the player can move.
 */
dynStoneArray findMovableStones(void) {

	int i;
	dynStoneArray ret;

	for (i = 0;
			i < sizeof(currentPlayer->stones) / sizeof(currentPlayer->stones[0]);
			i++) {
		if (currentPlayer->stones[i].alive
				&& movePossible(&currentPlayer->stones[i])) {
			stoneArray_append(ret, &currentPlayer->stones[i]);
		}
	}

	return ret;
}

/**
 * Find the stones the player can beat.
 */
dynStoneArray findBeatableStones(dynStoneArray *stones) {
	int i;
	dynStoneArray ret;

	for (i = 0; i < sizeof(stones) / sizeof(stones[0]); i++) {
		if (stones[i].alive && canBeatStone(stones[i])) {
			stoneArray_append(ret, stones[i]);
		}
	}

	return ret;
}

void computerDoMove(void) {
	stoneArray_init(&movableStones);

	movableStones = findMovableStones();
	beatableStones = findBeatableStones(movableStones);

}
