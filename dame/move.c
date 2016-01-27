#include <stdio.h>

#include "globals.h"
#include "move.h"

/*
void moveToRight() {
	clearSelectedField(selection);
	struct field *tmpSelection = selection;

	if (tmpSelection <= &(_pitch.fields[63]))
		tmpSelection++;

	while (tmpSelection <= &(_pitch.fields[63])) {

		if (isFieldValid(tmpSelection)) {
			selection = tmpSelection;
			break;
		}

		if (tmpSelection == &(_pitch.fields[63]))
			break;

		tmpSelection++;
	}
}

void moveToLeft() {
	clearSelectedField(selection);
	struct field *tmpSelection = selection;

	if (tmpSelection >= &(_pitch.fields[0]))
		tmpSelection--;

	while (tmpSelection >= &(_pitch.fields[0])) {

		if (isFieldValid(tmpSelection)) {
			selection = tmpSelection;
			break;
		}

		if (tmpSelection == &(_pitch.fields[0]))
			break;

		tmpSelection--;
	}
}

//
 1 = topRight
 2 = top
 3 = topLeft
 4 = left
 5 = downLeft
 6 = down
 7 = downRight
 8 = right
 */
/*int stepTo(int direction) {

	clearSelectedField(selection);
	struct field *tmpSelection = selection;

	switch (direction) {
	case 1:
		tmpSelection = tmpSelection->topRight;
		break;
	case 2:
		tmpSelection = tmpSelection->top;
		break;
	case 3:
		tmpSelection = tmpSelection->topLeft;
		break;
	case 4:
		tmpSelection = tmpSelection->left;
		break;
	case 5:
		tmpSelection = tmpSelection->bottomLeft;
		break;
	case 6:
		tmpSelection = tmpSelection->bottom;
		break;
	case 7:
		tmpSelection = tmpSelection->bottomRight;
		break;
	case 8:
		tmpSelection = tmpSelection->right;
		break;
	default:
		break;
	}

	struct stone * checkStone;
	struct player * checkPlayer;
	struct field * checkField;

	if (tmpSelection != NULL) {

		checkField = tmpSelection;
		checkStone = checkField->_stone;
		checkPlayer = checkStone->_player;

		if (&(checkPlayer->color) == &(currentPlayer->color)) {
			selection = tmpSelection;
			return 1;
		}

	}
	return 0;
}

int absOf2Numbers(int x, int y) {
	int result = 0;
	if (x > y) {
		result = x - y;
	} else {
		result = y - x;
	}
	return result;
}

int calcManhattanDistance( x1, x2, y1, y2) {
	return absOf2Numbers(x1, x2) + absOf2Numbers(y1, y2);
}

int findNearestStone(int indexCurrentField, int candidates[11]) {
	int currentBestCandidate = indexCurrentField;

	int currentLine = indexCurrentField / 8;
	int currentRow = indexCurrentField % 8;

	int candidateRow;
	int candidateLine;

	int shortestDist = 50;
	int newDist = -1;

	int x;
	for (x = 10; x >= 0; x--) {

		//no more possible fields -> abort at -1
		if (candidates[x] == -1) {
			break;
		}

		candidateLine = candidates[x] / 8;
		candidateRow = candidates[x] % 8;

		if (candidateLine != currentLine) {

			newDist = calcManhattanDistance(candidateRow, currentRow,
					candidateLine, currentLine);

			if (shortestDist > newDist) {
				shortestDist = newDist;
				currentBestCandidate = candidates[x];
			}

		} else {
			//no more stone above the current stone
			break;
		}
	}

	return currentBestCandidate;
}

int moveToTop() {

	struct field *tmpSelection = selection;
	//clearSelectedField(selection);

	if (tmpSelection != selectedStoneField) {

		if (stepTo(1))
			return 1;
		if (stepTo(3))
			return 1;

		struct stone * checkStone;
		struct player * checkPlayer;
		struct field * checkField;

		int i = 0;
		int arrayIndex = 10;
		int nearstStoneCandidates[11];

		//null-init of the result-array
		for (i = 0; i < 11; i++) {
			nearstStoneCandidates[i] = -1;
		}

		i = 0;
		while ((tmpSelection) != &(_pitch.fields[i])) {

			checkField = &(_pitch.fields[i]);
			checkStone = checkField->_stone;
			checkPlayer = checkStone->_player;

			if (&(checkPlayer->color) == &(currentPlayer->color)) {
				nearstStoneCandidates[arrayIndex] = i;
				arrayIndex--;
			}

			i++;
		}

		clearSelectedField(selection);
		int indexNearestStoneAbove = findNearestStone(i, nearstStoneCandidates);

		selection = &(_pitch.fields[indexNearestStoneAbove]);
		return 1;
	} else {
		//TODO: implement the routine going up
		if (isFieldValid(tmpSelection->topRight)) {
			selection = tmpSelection->topRight;
			return 1;
		}

		if (isFieldValid(tmpSelection->topLeft)) {
			selection = tmpSelection->topLeft;
			return 1;
		}
	}
	return 0;
}

int moveToBottom() {

	struct field *tmpSelection = selection;
	if (tmpSelection != selectedStoneField) {

		if (stepTo(5))
			return 1;
		if (stepTo(7))
			return 1;

		struct stone * checkStone;
		struct player * checkPlayer;
		struct field * checkField;

		int i = 0;
		int arrayIndex = 10;
		int nearstStoneCandidates[11];

		//null-init of the result-array
		for (i = 0; i < 11; i++) {
			nearstStoneCandidates[i] = -1;
		}

		i = 63;
		while ((tmpSelection) != &(_pitch.fields[i])) {

			checkField = &(_pitch.fields[i]);
			checkStone = checkField->_stone;
			checkPlayer = checkStone->_player;

			if (&(checkPlayer->color) == &(currentPlayer->color)) {
				nearstStoneCandidates[arrayIndex] = i;
				arrayIndex--;
			}

			i--;
		}

		clearSelectedField(selection);
		int indexNearestStoneAbove = findNearestStone(i, nearstStoneCandidates);

		selection = &(_pitch.fields[indexNearestStoneAbove]);
		return 1;
	} else {
		//TODO: implement the routine going up
		if (isFieldValid(tmpSelection->bottomRight)) {
			selection = tmpSelection->bottomRight;
			return 1;
		}

		if (isFieldValid(tmpSelection->bottomLeft)) {
			selection = tmpSelection->bottomLeft;
			return 1;
		}
	}
	return 0;
}
*/
