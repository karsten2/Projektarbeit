#include "globals.h"
#include "computer.h"

#include <stdio.h>
#include <stdlib.h>

struct stoneArray{
	int init;
	struct stone stone;
	struct stoneArray *next;
};

struct stoneArray newStoneArray(void) {
	return (struct stoneArray) { .stone = NULL, .next = NULL, .init = 0};
}

void stoneArrayAppend(struct stoneArray *ptr, struct stone new) {
	struct stoneArray temp;
	temp.init = 1;
	temp.stone = new;
	temp.next = NULL;

	ptr->next = &temp;
}

/**
 * Struct contains the current state of the board.
 * For each move of the MiniMax algorithm a new board has to be created.
 */
typedef struct {
	struct stoneArray stonesMax;
	struct stoneArray stonesMin;
	struct stoneArray moveList;
} board;

struct boardState{
	struct boardState *previous;
	struct boardState *next;
	struct boardState *up;
	struct boardState *down;
	board element;
	int player;
	int value;
};

struct boardState rootNode;

/**
 * Initializes a new boardstate.
 */
struct boardState newBoardState(void) {
	return (struct boardState) { .value = 0, .element = NULL, .down = NULL, .next = NULL,
		.previous = NULL, .up = NULL, .player = 0 };
}

/**
 * Adds Next Element in the tree.
 */
struct boardState * addNext(struct boardState *rootNode, board *nextElement) {
	struct boardState next = newBoardState();
	next.down = NULL;
	next.element = *nextElement;
	next.next = NULL;
	next.previous = NULL;
	next.up = rootNode->up;
	next.value = 0;

	if (rootNode->next == NULL) {
		rootNode->next = &next;
		next.previous = rootNode;
	} else {
		struct boardState *temp = rootNode->down;

		while (temp->next != NULL) {
			temp = temp->next;
		}

		next.previous = temp;
		temp->next = &next;
	}

	return &next;
}

/**
 * Function adds a new node at the bottom of the given rootNode.
 * Calls addNext to add the node at the correct position.
 */
struct boardState * addBottom(struct boardState *rootNode, board *bottomElement) {
	struct boardState bottom = newBoardState();
	bottom.down = NULL;
	bottom.element = *bottomElement;
	bottom.next = NULL;
	bottom.previous = NULL;
	bottom.up = rootNode;
	bottom.value = 0;

	if (rootNode->down == NULL)
		rootNode->down = &bottom;
	else
		return addNext(rootNode->down, bottomElement);

	return &bottom;
}

/**
 * Evaluate all values form the leafes to the root node.
 */
int evaluateSearchTree(struct boardState *rootNode) {

	struct boardState *tempNode = rootNode;

	if (tempNode->down != NULL)
		tempNode->value += evaluateSearchTree(tempNode->down);

	if (tempNode->next != NULL)
		tempNode->value += evaluateSearchTree(tempNode->next);

	return tempNode->value;
}

/**
 *
 */
void mmMoveStone(struct stone *stone, struct field *field) {
	field->_stone = stone;

	stone->_field = NULL;
	stone->_field->_stone = NULL;
}


/**
 * Check if the move of the player is valid.
 * If the player moves over an enemy stone, this stone will be beaten.
 *
 * dst: The field, the player wants to move the stone.
 */
int stoneCanMove(struct field *dst) {
	if (dst != NULL && dst->_stone == NULL) {
		return 1;
	}
	return 0;
}

/**
 * Function checks if a stone can beat another.
 */
int stoneCanBeat(struct field *dst, struct field *stoneToBeat,
		struct player *player) {

	if (dst != NULL && stoneToBeat->_stone != NULL
			&& !playerOwnsStone(player, stoneToBeat))
		return 1;

	return 0;
}

int movePossible(struct stone *stone) {
	if (stone->king) {
		if(stoneCanMove(stone->_field->topLeft)
				|| stoneCanMove(stone->_field->topRight)
				|| stoneCanMove(stone->_field->bottomLeft)
				|| stoneCanMove(stone->_field->bottomRight)) {
			return 1;
		}
	} else {
		if (stone->_player->position) {
			if (stoneCanMove(stone->_field->bottomLeft)
				|| stoneCanMove(stone->_field->bottomRight))
				return 1;
		} else {
			if (stoneCanMove(stone->_field->topLeft)
					|| stoneCanMove(stone->_field->topRight))
				return 1;
		}
	}

	return 0;
}

/**
 * Find all Stones the player can move.
 */
void findMovableStones(struct stoneArray *movableStones, struct stoneArray *stones) {

	struct stoneArray *temp = stones;

	while (temp != NULL) {
		if (temp->stone.alive && movePossible(&temp->stone)) {
			stoneArrayAppend(movableStones, temp->stone);
		}
		temp = temp->next;
	}
}

int beatPossible(struct stone *stone, struct player *player) {

	if (stone->king) {
		if ((stoneCanBeat(stone->_field->topLeft->topLeft,
				stone->_field->topLeft, player))
				|| (stoneCanBeat(stone->_field->topRight->topRight,
				stone->_field->topRight, player))
				|| (stoneCanBeat(stone->_field->bottomLeft->bottomLeft,
				stone->_field->bottomLeft, player))
				|| stoneCanBeat(stone->_field->bottomRight->bottomRight,
				stone->_field->bottomRight, player)) {
			return 1;
		}
	} else {
		if (stone->_player->position) {
			if ((stoneCanBeat(stone->_field->bottomLeft->bottomLeft,
					stone->_field->bottomLeft, player))
					|| (stoneCanBeat(stone->_field->bottomRight->bottomRight,
							stone->_field->bottomRight, player))) {
				return 1;
			}
		} else {
			if ((stoneCanBeat(stone->_field->topLeft->topLeft,
					stone->_field->topLeft, player))
					|| (stoneCanBeat(stone->_field->topRight->topRight,
							stone->_field->topRight, player))) {
				return 1;
			}
		}
	}

	return 0;
}


/**
 * Find the stones the player can beat.
 */
void findBeatableStones(struct stoneArray *stones, struct stoneArray *possibleStones) {
	struct stoneArray *temp = possibleStones;

	while(temp != NULL) {
		if (temp->stone.alive
				&& beatPossible(&temp->stone, temp->stone._player)) {
			stoneArrayAppend(stones, temp->stone);
		}
		temp = temp->next;
	}
}

/**
 * Function returns a list with movable and beatable stones.
 */
struct stoneArray getMoveList(struct stoneArray stonesMax, struct stoneArray stonesMin) {
	struct stoneArray moveList = newStoneArray();

	findMovableStones(&moveList, &stonesMax);
	findBeatableStones(&moveList, &moveList);

	return moveList;
}

/**
 * Returns a new board struct.
 *
 * stonesMax:		All stones of the Max player.
 * stonesMin:		All stones of the Min player.
 * moveList:		All stones the player can move or beat.
 * value:			Value of the move.
 */
board newBoard(struct stoneArray stonesMax,
		struct stoneArray stonesMin) {

	struct stoneArray moveList = getMoveList(stonesMax, stonesMin);

	return (board) { .moveList = moveList, .stonesMax = stonesMax, .stonesMin =
			stonesMin};
}



struct player * getPlayerByPosition(int position) {
	int i;
	for (i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		if (players[i].position == position)
			return &players[i];
	}

	return currentPlayer;
}

/**
 * Function moves a stone and returns a new board with the new state.
 *
 * board:		current game state.
 * stone:		stone to move.
 * field:		field the stone will be moved to.
 */
board boardMove(board board, struct stone src, struct field dst) {
	mmMoveStone(&src, &dst);
	return newBoard(board.stonesMax, board.stonesMin);
}

/**
 * Function removes a stone from the pitch.
 */
void mmBeatStone(struct stone *_stone) {
	if (_stone != NULL) {
		_stone->alive = 0;
		_stone->_field->_stone = NULL;
		_stone->_field = NULL;
	}
}

/**
 * Function returns the other player.
 */
struct player * getPlayer(int position) {

	int i;
	for (i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		if (players[i].position == position) {
			return &players[i];
		}
	}

	return currentPlayer;
}

int getNextPlayer(int currentPosition) {

	int i;
	for (i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		if (players[i].position != currentPosition)
			return players[i].position;
	}

	return 0;
}

/**
 * Function to find recursive all possible moves end evaluate the best way to win.
 *
 * depth: 		Max depth of the search.
 * board: 		Current state of the game.
 * player: 		The turn of the current player, 1 max, -1 min
 */
void minimax(int depth, board _board, int player, struct boardState *rootNode) {

	// TODO Minimax: get movable or beatable stones

	if (depth == 0) {
		evaluateSearchTree(rootNode);
	} else {
		board tempBoard;
		struct boardState *tempBoardState;
		rootNode->element = _board;
		struct stoneArray *tempStoneArray = &_board.moveList;
		struct stone tempStone;

		while (tempStoneArray != NULL) {
			tempStone = tempStoneArray->stone;
			if (tempStone.king) {
				if (stoneCanMove(tempStone._field->topLeft)) {

					tempBoard = boardMove(_board, tempStone, *tempStone._field->topLeft);
					tempBoardState = addBottom(rootNode, &tempBoard);

					minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

				}
				if (stoneCanMove(tempStone._field->topRight)) {

					tempBoard = boardMove(_board, tempStone, *tempStone._field->topRight);
					tempBoardState = addBottom(rootNode, &tempBoard);

					minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

				}
				if (stoneCanMove(tempStone._field->bottomLeft)) {

					tempBoard = boardMove(_board, tempStone, *tempStone._field->bottomLeft);
					tempBoardState = addBottom(rootNode, &tempBoard);

					minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

				}
				if (stoneCanMove(tempStone._field->bottomRight)) {

					tempBoard = boardMove(_board, tempStone, *tempStone._field->bottomLeft);
					tempBoardState = addBottom(rootNode, &tempBoard);

					minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

				}
				if (stoneCanBeat(tempStone._field->topLeft->topLeft,
						tempStone._field->topLeft, currentPlayer)) {
				}
				if (stoneCanBeat(tempStone._field->topRight->topRight,
						tempStone._field->topRight, currentPlayer)) {
				}
				if (stoneCanBeat(tempStone._field->bottomLeft->bottomLeft,
						tempStone._field->bottomLeft, currentPlayer)) {
				}
				if (stoneCanBeat(tempStone._field->bottomRight->bottomRight,
						tempStone._field->bottomRight, currentPlayer)) {
				}
			} else {
				if (player) {
					if (stoneCanMove(tempStone._field->bottomLeft)) {

						tempBoard = boardMove(_board, tempStone, *tempStone._field->bottomLeft);
						tempBoardState = addBottom(rootNode, &tempBoard);

						minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

					}
					if (stoneCanMove(tempStone._field->bottomRight)) {

						tempBoard = boardMove(_board, tempStone, *tempStone._field->bottomLeft);
						tempBoardState = addBottom(rootNode, &tempBoard);

						minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

					}
					if (stoneCanBeat(tempStone._field->bottomLeft->bottomLeft,
							tempStone._field->bottomLeft, currentPlayer)) {
					}
					if (stoneCanBeat(tempStone._field->bottomRight->bottomRight,
							tempStone._field->bottomRight, currentPlayer)) {
					}
				} else {
					if (stoneCanMove(tempStone._field->topLeft)) {

						tempBoard = boardMove(_board, tempStone, *tempStone._field->topLeft);
						tempBoardState = addBottom(rootNode, &tempBoard);

						minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

					}
					if (stoneCanMove(tempStone._field->topRight)) {

						tempBoard = boardMove(_board, tempStone, *tempStone._field->topRight);
						tempBoardState = addBottom(rootNode, &tempBoard);

						minimax(depth - 1, tempBoard, getNextPlayer(player), tempBoardState);

					}

					if (stoneCanBeat(tempStone._field->topLeft->topLeft,
							tempStone._field->topLeft, currentPlayer)) {
					}
					if (stoneCanBeat(tempStone._field->topRight->topRight,
							tempStone._field->topRight, currentPlayer)) {
					}
				}
			}

			tempStoneArray = tempStoneArray->next;
		}
	}
}

struct stoneArray * getLastElement(struct stoneArray **ptr) {

	if (&ptr->next != NULL)
		return *getLastElement(*ptr->next);
	else
		return *ptr;
}

/**
 * Function initializes the minimax Depth search.
 */
void computerStart(void) {

	int i;

	struct player *p1 = getPlayer(1);
	struct player *p2 = getPlayer(0);

	struct stoneArray stonesP1 = newStoneArray();
	struct stoneArray stonesP2 = newStoneArray();

	for (i = 0; i < sizeof(players[0].stones)/sizeof(players[0].stones[0]); i++) {
		if(!stonesP1.init) {
			stonesP1.init = 1;
			stonesP1.stone = players[0].stones[i];
			stonesP1.next = NULL;
		} else {

			struct stoneArray new;
			new.init = 1;
			new.stone = players[0].stones[i];
			new.next = NULL;


			struct stoneArray *temp = getLastElement(&stonesP1);
			temp->next = &new;
		}
	}

	for (i = 0; i < sizeof(players[1].stones)/sizeof(players[1].stones[0]); i++) {
		if(!stonesP2.init) {
			stonesP2.init = 1;
			stonesP2.stone = players[1].stones[i];
			stonesP2.next = NULL;
		} else {
			struct stoneArray *temp = &stonesP2;
			while (temp->next != NULL) { temp = temp->next; };
			stoneArrayAppend(temp, players[1].stones[i]);
		}
	}

	rootNode = newBoardState();
	board board = newBoard(stonesP1, stonesP2);

	minimax(3, board, 1, &rootNode);
}
