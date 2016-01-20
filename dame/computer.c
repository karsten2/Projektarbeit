#include "globals.h"
#include "computer.h"

#include <stdio.h>
#include <stdlib.h>

	// TODO Decide what move computer will do.
	// TODO Find the path in the tree, the enemy player chose.
	// TODO Do the ai move.
	// TODO Include uCos libs
	// TODO Create Tasks

enum strategy {
	min,
	max
};

typedef struct {
	int col;
	int row;
} aiField;

typedef struct {
	int king;
	int alive;
	aiField field;
	int position;
	struct stone *refStone;
} aiStone;

typedef struct {
	int Position;
	enum strategy strategy;
} aiPlayer;

/**
 * Struct contains the current state of the board.
 * For each move of the MiniMax algorithm a new board has to be created.
 */
typedef struct board {
	aiStone topStones[12];
	aiStone botStones[12];
	aiPlayer player;
	int isLeaf;
	struct board *parent;
	struct board *childs[50];
	int childsize;
	int nodeValue;
} board;

/**
 * Function to check if the game is over.
 * If the game is over, the function returns 0, 1 for the player.
 * If the game is not over, the function returns -1.
 *
 * @param world Struct of board.
 * @return 0, 1 Position of the winning player, else -1.
 */
int gameOver(board *world) {

	int allTopDead = 1;
	int allBotDead = 1;
	int i;

	for (i = 0; i < 12; i++) {
		if (world->topStones[i].alive)
			allTopDead = 0;
	}

	for (i = 0; i < 12; i++) {
		if (world->botStones[i].alive)
			allBotDead = 0;
	}

	if (allTopDead && !allBotDead)
		return 0;
	else if (!allTopDead && allBotDead)
		return 1;
	else
		return -1;
}

/**
 * Function to get a stone at a specific position.
 *
 * @param col Value between 1-8 for the column.
 * @param row Value between 1-8 for the row.
 * @param *world Pointer to the current board.
 *
 * @return Pointer to the found stone, else NULL.
 */
aiStone * getStoneAt(int col, int row, board *world) {
	int i;

	for (i = 0; i < 12; i++) {
		if (world->topStones[i].field.col == col && world->topStones[i].field.row == row)
			return &world->topStones[i];
	}
	for (i = 0; i < 12; i++) {
		if (world->botStones[i].field.col == col && world->botStones[i].field.row == row)
			return &world->botStones[i];
	}

	return NULL;
}

/**
 *
 */
int validateNewCoord(board *world, int col, int row) {
	if (col > 0 && col <= 8
			&& row > 0 && row <= 8) {
		if (getStoneAt(col, row, world))
			return 0;
		else
			return 1;
	} else
		return 0;
}

/**
 * 1 if bottomLeft is free.
 */
int bottomLeft(board *world, aiStone *stone) {
	int newCol = stone->field.col - 1;
	int newRow = stone->field.row + 1;

	return validateNewCoord(world, newCol, newRow);
}

aiField getBottomLeft(aiStone *stone) {

	aiField retValue;

	retValue.col = stone->field.col - 1;
	retValue.row = stone->field.row + 1;

	return retValue;
}

/**
 * 1 if bottomRight is free.
 */
int bottomRight(board *world, aiStone *stone) {
	int newCol = stone->field.col + 1;
	int newRow = stone->field.row + 1;

	return validateNewCoord(world, newCol, newRow);
}

aiField getBottomRight(aiStone *stone) {

	aiField retValue;

	retValue.col = stone->field.col + 1;
	retValue.row = stone->field.row + 1;

	return retValue;
}

/**
 * 1 if topLeft is free.
 */
int topLeft(board *world, aiStone *stone) {
	int newCol = stone->field.col - 1;
	int newRow = stone->field.row - 1;

	return validateNewCoord(world, newCol, newRow);
}

aiField getTopLeft(aiStone *stone) {

	aiField retValue;

	retValue.col = stone->field.col - 1;
	retValue.row = stone->field.row - 1;

	return retValue;
}

/**
 * 1 if topRight is free.
 */
int topRight(board *world, aiStone *stone) {
	int newCol = stone->field.col + 1;
	int newRow = stone->field.row - 1;

	return validateNewCoord(world, newCol, newRow);
}

aiField getTopRight(aiStone *stone) {

	aiField retValue;

	retValue.col = stone->field.col + 1;
	retValue.row = stone->field.row - 1;

	return retValue;
}

/**
 * Checks if field lays within the board.
 */
int fieldValid (aiField field) {
	if (field.col > 0 && field.col <= 8
			&& field.row > 0 && field.row <= 8)
		return 1;

	return 0;
}

/**
 * Checks if a stone can hit a victim.
 */
int stoneCanHitVictim(board *world, aiStone *victim, aiField dir) {

	if (!getStoneAt(dir.col, dir.row, world))
		return 1;

	return 0;
}

/**
 * Function checks if a given stone has an enemy stone at the given field.
 */
aiStone * getEnemyNeighbour(board *world, aiStone *stone,aiField dir) {

	aiStone *victim = getStoneAt(dir.col, dir.row, world);
	if (victim && victim->position != stone->position)
		return victim;

	return NULL;
}

void appendChild(board *world, board *child) {

	// adds child to childarray, increases array counter.s
	world->childs[world->childsize] = child;
	world->childsize ++;

	// node is no leaf anymore, after adding a child.
	if (world->isLeaf)
		world->isLeaf = 0;

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

/**
 * Function to get the next player.
 *
 * @param currentPlayer The player of the current turn.
 * @return The next player.
 */
aiPlayer getNextPlayer(aiPlayer currentPlayer) {

	int retPosition;
	enum strategy retStrategy;

	if (currentPlayer.Position)
		retPosition = 0;
	else
		retPosition = 1;

	if (currentPlayer.strategy == max)
		retStrategy = min;
	else
		retStrategy = max;

	return (aiPlayer) { .Position = retPosition, .strategy = retStrategy };
}

/**
 * Copies a board.
 *
 * @param *src Pointer to source board.
 * @param *dst Pointer to the destination board.
 */
void copyBoard(board *src, board *dst) {
	memcpy(dst, src, sizeof(board));
}

/**
 * Function to simulate a move for the ai.
 *
 * @param *world Pointer to the current World.
 * @param *stoneToMove Pointer to the moving stone.
 * @param newField The destination after the move.
 *
 * @return The new world as board.
 */
board doMove(board *world, aiStone *stoneToMove, aiField newField) {

	int i;
	board retValue;
	aiStone *ptrStone = malloc(12 * sizeof(aiStone));

	copyBoard(world, &retValue);
	retValue.player = getNextPlayer(retValue.player);
	retValue.nodeValue = -1;

	if (stoneToMove->position)
		ptrStone = retValue.topStones;
	else
		ptrStone = retValue.botStones;

	for (i = 0; i < 12; i++) {

		if (ptrStone->field.col == stoneToMove->field.col &&
				ptrStone->field.row == stoneToMove->field.row) {
			ptrStone->field = newField;

			// Transform the moved stone to a king.
			if (ptrStone->position) {
				if (ptrStone->field.row == 8)
					ptrStone->king = 1;
			} else {
				if (ptrStone->field.row == 1)
					ptrStone->king = 1;
			}

			break;
		}

		ptrStone ++;
	}

	return retValue;
}

/**
 * Beat a stone with a given world.
 *
 * @param *world Pointer to the current world.
 * @param *stone The hitting stone.
 * @param *victim The beaten stone.
 * @param newField The field the hitting stone will be after the hit.
 *
 * @return The new world without the victim.
 */
board hitStone(board *world, aiStone *stone, aiStone *victim, aiField newField) {

	// remove Victim.
	board retValue;
	copyBoard(world, &retValue);
	retValue.player = getNextPlayer(retValue.player);
	retValue.nodeValue = -1;

	aiStone *newVictim = getStoneAt(victim->field.col, victim->field.row, &retValue);
	newVictim->alive = 0;

	// move hitting stone to new fields.
	aiStone *newStone = getStoneAt(stone->field.col, stone->field.row, &retValue);
	newStone->field = newField;

	return retValue;
}

/**
 * Function checks if a stone can hit another stone.
 *
 * @param *world Pointer to the current world.
 * @param *stone The stone to check.
 *
 * @return 1 if stone can hit, else 0.
 */
int canHit(board *world, aiStone *stone) {

	aiField botLeft = getBottomLeft(stone);
	aiField botRight = getBottomRight(stone);
	aiField tLeft = getTopLeft(stone);
	aiField tRight = getTopRight(stone);
	aiStone *victim;

	if (stone->alive && (stone->position || stone->king)) {
		victim = getEnemyNeighbour(world, stone, botLeft);

		if (victim && bottomLeft(world, victim))
			return 1;

		victim = getEnemyNeighbour(world, stone, botRight);
		if (victim && bottomRight(world, victim))
			return 1;
	}

	if (stone->alive && (!stone->position || stone->king)) {
		victim = getEnemyNeighbour(world, stone, tLeft);
		if (victim && topLeft(world, victim))
			return 1;

		victim = getEnemyNeighbour(world, stone, tRight);
		if (victim && topRight(world, victim))
			return 1;
	}

	return 0;
}

/**
 * Function to evaluate a world.
 * Stone values:
 * 		Normal Stone: 						+ 100.
 * 		King Stone:							+ 200.
 * 		Each Row closer to Kings Row:		+ 10 * Row -> row 7: 100 + 7 * 10 = 170.
 * 		Stone can Hit:						+ 100.
 * 		Stone is at col 1 or 8 (cant die):	+ 50.
 * 		Game won:							+ 1000.
 *
 * @param *world The current world.
 */
void evaluateWorld(board *world) {

	aiStone *ptr = world->topStones;
	int i;
	int countMax = 0;
	int countMin = 0;

	for (i = 0; i < 12; i++) {
		if (ptr->alive) {

			if (ptr->king)
				countMax += 200;
			else
				countMax += 100;

			// closer to kingsrow
			countMax += ptr->field.row * 10;

			// is at border (cant die)
			if (ptr->field.col == 1 || ptr->field.col == 8)
				countMax += 50;

			if (canHit(world, ptr))
				countMax += 100;
		}
		ptr ++;
	}

	ptr = world->botStones;
	for (i = 0; i < 12; i++) {
		if (ptr->alive) {

			if (ptr->king) {
				countMin += 200;
			} else {
				countMin += 100;
			}

			// closer to kingsrow
			countMin += ptr->field.row * 10;

			// is at border (cant die)
			if (ptr->field.col == 1 || ptr->field.col == 8) {
				countMin += 50;
			}

			// can Hit
			if (canHit(world, ptr))
				countMin += 100;
		}
		ptr ++;
	}

	int gameover = gameOver(world);
	switch(gameover) {
		case 0:
			countMin += 1000;
			break;
		case 1:
			countMax += 1000;
			break;
	}

	switch (world->player.strategy) {
		case min:
			world->nodeValue = countMin - countMax;
			break;
		case max:
			world->nodeValue = countMax - countMin;
			break;
	}
}

/**
 * Function to find recursive all possible moves and hitchances.
 *
 * depth: 		Max depth of the search.
 * board: 		Current state of the game.
 *
 * @param depth The search depth.
 * @param *world The current world.
 */
void depthSearch(int depth, board *world) {

	int i;

	aiStone *curStone;
	if (world->player.Position)
		curStone = world->topStones;
	else
		curStone = world->botStones;

	if (depth > 0) {
		for(i = 0; i < 12; i++) {
			if (curStone->alive && (curStone->position || curStone->king)) {
				if (bottomLeft(world, curStone)) {

					// move the stone bottom left.
					board newWorld = doMove(world, curStone, getBottomLeft(curStone));

					appendChild(world, &newWorld);
					newWorld.parent = world;

					depthSearch(depth - 1, &newWorld);

				} else {
					aiField botLeft = getBottomLeft(curStone);

					// check hitchance bottom left.
					aiStone *victim = getEnemyNeighbour(world, curStone, botLeft);
					if (victim && stoneCanHitVictim(world, victim, botLeft)) {
						board newWorld = hitStone(world, curStone, victim, getBottomLeft(victim));

						appendChild(world, &newWorld);
						newWorld.parent = world;

						depthSearch(depth - 1, &newWorld);
					}
				}

				if (bottomRight(world, curStone)) {

					// move the stone bottom right.
					board newWorld = doMove(world, curStone, getBottomRight(curStone));

					appendChild(world, &newWorld);
					newWorld.parent = world;

					depthSearch(depth - 1, &newWorld);

				} else {
					aiField botRight = getBottomRight(curStone);

					// check hitchance bottom left.
					aiStone *victim = getEnemyNeighbour(world, curStone, botRight);
					if (victim && stoneCanHitVictim(world, victim, botRight)) {
						board newWorld = hitStone(world, curStone, victim, getBottomRight(victim));

						appendChild(world, &newWorld);
						newWorld.parent = world;

						depthSearch(depth - 1, &newWorld);
					}
				}
			}
			if (curStone->alive && (!curStone->position || curStone->king)) {
				if (topLeft(world, curStone)) {

					// move the stone top left.
					board newWorld = doMove(world, curStone, getTopLeft(curStone));

					appendChild(world, &newWorld);
					newWorld.parent = world;

					depthSearch(depth - 1, &newWorld);

				} else {
					// check hitchance
					aiField topLeft = getTopLeft(curStone);

					// check hitchance bottom left.
					aiStone *victim = getEnemyNeighbour(world, curStone, topLeft);
					if (victim && stoneCanHitVictim(world, victim, topLeft)) {
						board newWorld = hitStone(world, curStone, victim, getTopLeft(victim));

						appendChild(world, &newWorld);
						newWorld.parent = world;

						depthSearch(depth - 1, &newWorld);
					}
				}
				if (topRight(world, curStone)) {

					// move the stone top right.
					board newWorld = doMove(world, curStone, getTopRight(curStone));

					appendChild(world, &newWorld);
					newWorld.parent = world;

					depthSearch(depth - 1, &newWorld);

				} else {
					// check hitchance
					aiField topRight = getTopRight(curStone);

					// check hitchance bottom left.
					aiStone *victim = getEnemyNeighbour(world, curStone, topRight);
					if (victim && stoneCanHitVictim(world, victim, topRight)) {
						board newWorld = hitStone(world, curStone, victim, getTopRight(victim));

						appendChild(world, &newWorld);
						newWorld.parent = world;

						depthSearch(depth - 1, &newWorld);
					}
				}
			}

			curStone ++;
		}
	}
}

/**
 * converts struct stone to aiStone to remove pointers.
 */
aiStone stoneToAiStone(struct stone *src) {
	aiStone retValue;

	retValue.alive = src->alive;
	retValue.king = src->king;
	retValue.position = src->_player->position;
	retValue.refStone = src;
	retValue.field.col = src->_field->col;
	retValue.field.row = src->_field->row;

	return retValue;
}

/**
 * Analyzes the value of all childnodes.
 * If the strategy is Max:
 * 		the highest Node value of all Childnodes
 * 		is the new Value of the parent node.
 *
 * If the strategy is Min:
 * 		the lowest node value of all childnodes
 * 		is the new value of the parent node.
 *
 * @param *node The current world.
 */
void evaluateNode(board *node) {
	int temp = -1;
	int i;

	if (node->player.strategy == max) {
		// get the MAX value from the leafes.
		for (i = 0; i < node->childsize; i++) {
			if (temp == -1)
				temp = node->childs[i]->nodeValue;

			if (node->childs[i]->nodeValue > temp)
				temp = node->childs[i]->nodeValue;
		}
	} else {
		// get the MIN value from the leafes.
		for (i = 0; i < node->childsize; i++) {
			if (temp == -1)
				temp = node->childs[i]->nodeValue;

			if (node->childs[i]->nodeValue < temp)
				temp = node->childs[i]->nodeValue;
		}
	}

	node->nodeValue = temp;
}

/**
 * Traverse through the searchtree.
 * If the search reaches a leaf (no Childnodes left),
 * the evaluation function is called.
 *
 * @param *node The current world.
 */
void miniMax(board *node) {

	int i;

	if (node->isLeaf) {

		evaluateWorld(node);

	} else if (node->childsize) {
		for (i = 0; i < node->childsize; i++) {

			miniMax(node->childs[i]);
		}

		evaluateNode(node);
	}
}



/**
 * Function initializes the minimax Depth search.
 */
void computerStart(void) {

	int i = 0;
	board world;

	// Create a new world.
	world.player.Position = 1;
	world.player.strategy = max;
	world.childsize = 0;
	world.isLeaf = 1;
	world.nodeValue = -1;

	struct stone *ptrStones = players[1].stones;

	while (i < 12) {
		world.topStones[i] = stoneToAiStone(ptrStones);
		ptrStones ++;
		i ++;
	}

	ptrStones = players[0].stones;
	i = 0;

	while (i < 12) {
		world.botStones[i] = stoneToAiStone(ptrStones);
		ptrStones ++;
		i ++;
	}

	// create a search tree via depth search.
	depthSearch(1, &world);

	// traverse the tree and evaluate the leafes.
	// evaluate all nodes with the minimax algorithm.
	miniMax(&world);

	// Decide what move the computer will do.

}
