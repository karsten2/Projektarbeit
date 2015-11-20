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
	return 0;
}



/**
 * Find all Stones the player can move.
 */
/*
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
*/
/**
 * Find the stones the player can beat.
 */
/*
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
*/
void computerDoMove(void) {
	stoneArray_init(&movableStones);
/*
	movableStones = findMovableStones();
	beatableStones = findBeatableStones(movableStones);*/

}
