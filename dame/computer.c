#include "globals.h"

#include <stdio.h>
#include <stdlib.h>

// TODO Decide what move computer will do.
// TODO Find the path in the tree, the enemy player chose.
// TODO Do the ai move.
// TODO Create task structure

static const struct aiStone emptyStone;

/**
 * Struct contains the current state of the board.
 * For each move of the MiniMax algorithm a new board has to be
 created.
 */
struct board {
	struct aiStone topStones[12];
	struct aiStone botStones[12];
	int player;
	struct aiMove moves[24];
	int moveCount;
	int nodeValue;
};
/**
 * Function to check if the game is over.
 * If the game is over, the function returns 0, 1 for the player.
 * Else: the function returns -1.
 *
 * @param world Struct of board.
 * @return 0, 1 Position of the winning player, else -1.
 */
int gameOver(struct board *world) {

	int allTopDead = 1;
	int allBotDead = 1;
	int i;

	for (i = 0; i < 12; i++) {
		if (world->topStones[i].alive) {
			allTopDead = 0;
			break;
		}
	}

	for (i = 0; i < 12; i++) {
		if (world->botStones[i].alive) {
			allBotDead = 0;
			break;
		}
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
struct aiStone * getStoneAt(int col, int row, struct board *world) {
	int i;

	for (i = 0; i < 12; i++) {
		if (world->topStones[i].field.col == col
				&& world->topStones[i].field.row == row)
			return &world->topStones[i];
	}
	for (i = 0; i < 12; i++) {
		if (world->botStones[i].field.col == col
				&& world->botStones[i].field.row == row)
			return &world->botStones[i];
	}

	return NULL;
}

/**
 *
 */
int validateNewCoord(struct board *world, int col, int row) {
	if (col > 0 && col <= 8 && row > 0 && row <= 8) {
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
int bottomLeft(struct board *world, struct aiStone *stone) {
	int newCol = stone->field.col - 1;
	int newRow = stone->field.row + 1;

	return validateNewCoord(world, newCol, newRow);
}

struct aiField getBottomLeft(struct aiStone *stone) {

	struct aiField retValue;

	retValue.col = stone->field.col - 1;
	retValue.row = stone->field.row + 1;

	return retValue;
}

/**
 * 1 if bottomRight is free.
 */
int bottomRight(struct board *world, struct aiStone *stone) {
	int newCol = stone->field.col + 1;
	int newRow = stone->field.row + 1;

	return validateNewCoord(world, newCol, newRow);
}

struct aiField getBottomRight(struct aiStone *stone) {

	struct aiField retValue;

	retValue.col = stone->field.col + 1;
	retValue.row = stone->field.row + 1;

	return retValue;
}

/**
 * 1 if topLeft is free.
 */
int topLeft(struct board *world, struct aiStone *stone) {
	int newCol = stone->field.col - 1;
	int newRow = stone->field.row - 1;

	return validateNewCoord(world, newCol, newRow);
}

struct aiField getTopLeft(struct aiStone *stone) {

	struct aiField retValue;

	retValue.col = stone->field.col - 1;
	retValue.row = stone->field.row - 1;

	return retValue;
}

/**
 * 1 if topRight is free.
 */
int topRight(struct board *world, struct aiStone *stone) {
	int newCol = stone->field.col + 1;
	int newRow = stone->field.row - 1;

	return validateNewCoord(world, newCol, newRow);
}

struct aiField getTopRight(struct aiStone *stone) {

	struct aiField retValue;

	retValue.col = stone->field.col + 1;
	retValue.row = stone->field.row - 1;

	return retValue;
}

/**
 * Checks if field lays within the board.
 */
int fieldValid(struct aiField field) {
	if (field.col > 0 && field.col <= 8 && field.row > 0 && field.row <= 8)
		return 1;

	return 0;
}

/**
 * Checks if a stone can hit a victim.
 */
int stoneCanHitVictim(struct board *world, struct aiStone *victim,
		struct aiField dir) {

	if (!getStoneAt(dir.col, dir.row, world))
		return 1;

	return 0;
}

/**
 * Function checks if a given stone has an enemy stone at the given
 field.
 */
struct aiStone * getEnemyNeighbour(struct board *world, struct aiStone *stone,
		struct aiField dir) {

	struct aiStone *victim = getStoneAt(dir.col, dir.row, world);
	if (victim && victim->position != stone->position)
		return victim;

	return NULL;
}

/**
 * Function returns the other player.
 */
int getNextPlayer(int position) {

	/*int i;
	for (i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
		if (players[i].position != position) {
			return players[i].position;
		}
	}*/

	return 0;
}

/**
 * Copies a board.
 *
 * @param *src Pointer to source board.
 * @param *dst Pointer to the destination board.
 */
void copyBoard(struct board *src, struct board *dst) {
	memcpy(dst, src, sizeof(struct board));
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
struct board doMove(struct board *world, struct aiStone *stoneToMove,
		struct aiField newField) {

	int i;
	struct board retValue;

	struct aiStone *ptrStone = malloc(12 * sizeof(struct aiStone));

	copyBoard(world, &retValue);
	retValue.player = getNextPlayer(retValue.player);
	retValue.nodeValue = -1;

	if (stoneToMove->position)
		ptrStone = retValue.topStones;
	else
		ptrStone = retValue.botStones;

	for (i = 0; i < 12; i++) {

		if (ptrStone->field.col == stoneToMove->field.col
				&& ptrStone->field.row == stoneToMove->field.row) {
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

		ptrStone++;
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
struct board hitStone(struct board *world, struct aiStone *stone,
		struct aiStone *victim, struct aiField newField) {

	// remove Victim.
	struct board retValue;
	copyBoard(world, &retValue);
	retValue.player = getNextPlayer(retValue.player);
	retValue.nodeValue = -1;

	struct aiStone *newVictim = getStoneAt(victim->field.col, victim->field.row,
			&retValue);
	newVictim->alive = 0;

	// move hitting stone to new fields.
	struct aiStone *newStone = getStoneAt(stone->field.col, stone->field.row,
			&retValue);
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
int canHit(struct board *world, struct aiStone *stone) {

	struct aiField botLeft = getBottomLeft(stone);
	struct aiField botRight = getBottomRight(stone);
	struct aiField tLeft = getTopLeft(stone);
	struct aiField tRight = getTopRight(stone);
	struct aiStone *victim;

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
 * Function to evaluate 12 stones of a player.
 * Stone values:
 *              Normal Stone:
 + 100.
 *              King Stone:
 + 200.
 *              Each Row closer to Kings Row:           + 10 *
 Row -> row 7: 100 + 7 * 10 = 170.
 *              Stone can Hit:
 + 100.
 *              Stone is at col 1 or 8 (cant die):      + 50.
 *              Game won:
 + 1000.
 *
 * @param *world The current world.
 */
int evaluateStones(struct board *world, struct aiStone *ptr, int player) {

	int i;
	long returnValue = 0;

	for (i = 0; i < 12; i++) {
		if (ptr->alive) {

			if (ptr->king)
				returnValue += 200;
			else
				returnValue += 100;

			// closer to kingsrow
			returnValue += ptr->field.row * 10;

			// is at border (cant die)
			if (ptr->field.col == 1 || ptr->field.col == 8)
				returnValue += 50;

			if (canHit(world, ptr))
				returnValue += 100;
		}
		ptr++;
	}

	int gameover = gameOver(world);
	switch (gameover) {
	case 0:
	case 1:
		if (gameover == player)
			returnValue += 1000;
		break;
	}

	return (int) returnValue;
}

void createWorldFromMove(struct board *world, struct aiMove *move) {

	struct board newWorld;
	int valTop, valBot, valResult;

	if (!move->beatStone)
		newWorld = doMove(world, &move->Hitter, move->Destination);
	else
		newWorld = hitStone(world, &move->Hitter, &move->Victim,
				move->Destination);

	struct aiStone *ptr = newWorld.topStones;
	valTop = evaluateStones(&newWorld, ptr, newWorld.player);

	ptr = newWorld.botStones;
	valBot = evaluateStones(&newWorld, ptr, newWorld.player);

	valResult = valBot - valTop;
	move->value = valResult;
}

void appendMove(struct board *world, struct aiStone hitter,
		struct aiStone victim, struct aiField destination, int beatStone) {

	struct aiMove newMove = (struct aiMove ) { .Hitter = hitter, .Victim =
					victim, .Destination = destination, .value = -1,
					.beatStone = beatStone };
	createWorldFromMove(world, &newMove);
	world->moves[world->moveCount] = newMove;
	world->moveCount++;
}

/**
 * Function to find recursive all possible moves and hitchances.
 *
 * depth:               Max depth of the search.
 * board:               Current state of the game.
 *
 * @param depth The search depth.
 * @param *world The current world.
 */
void depthSearch(int depth, struct board *world) {

	int i;

	struct aiStone *curStone;
	if (world->player)
		curStone = world->topStones;
	else
		curStone = world->botStones;

	if (depth > 0) {
		for (i = 0; i < 12; i++) {
			if (curStone->alive && (curStone->position || curStone->king)) {
				if (bottomLeft(world, curStone)) {

					appendMove(world, *curStone, emptyStone,
							getBottomLeft(curStone), 0);

				} else {
					struct aiField botLeft = getBottomLeft(curStone);

					// check hitchance bottom left.
					struct aiStone *victim = getEnemyNeighbour(world, curStone,
							botLeft);
					if (victim && stoneCanHitVictim(world, victim, botLeft)) {

						/*child2 =
						 hitStone(world, curStone, victim, getBottomLeft(victim));

						 appendChild(world,
						 &child2);
						 depthSearch(depth - 1,
						 &child2);*/
					}
				}

				if (bottomRight(world, curStone)) {

					// move the stone bottom right.
					appendMove(world, *curStone, emptyStone,
							getBottomLeft(curStone), 0);

				} else {
					struct aiField botRight = getBottomRight(curStone);

					// check hitchance bottom left.
					struct aiStone *victim = getEnemyNeighbour(world, curStone,
							botRight);
					if (victim && stoneCanHitVictim(world, victim, botRight)) {
						/*child4 =
						 hitStone(world, curStone, victim, getBottomRight(victim));

						 appendChild(world,
						 &child4);

						 depthSearch(depth - 1,
						 &child4);*/
					}
				}
			}
			if (curStone->alive && (!curStone->position || curStone->king)) {
				if (topLeft(world, curStone)) {

					// move the stone top left.
					/*child5 = doMove(world,
					 curStone, getTopLeft(curStone));

					 appendChild(world, &child5);

					 depthSearch(depth - 1,
					 &child5);*/

				} else {
					// check hitchance
					struct aiField topLeft = getTopLeft(curStone);

					// check hitchance bottom left.
					struct aiStone *victim = getEnemyNeighbour(world, curStone,
							topLeft);
					if (victim && stoneCanHitVictim(world, victim, topLeft)) {

						/*child6 =
						 hitStone(world, curStone, victim, getTopLeft(victim));

						 appendChild(world,
						 &child6);
						 depthSearch(depth - 1,
						 &child6);*/
					}
				}
				if (topRight(world, curStone)) {

					// move the stone top right.
					/*child7 = doMove(world,
					 curStone, getTopRight(curStone));

					 appendChild(world, &child7);

					 depthSearch(depth - 1,
					 &child7);*/

				} else {
					// check hitchance
					struct aiField topRight = getTopRight(curStone);

					// check hitchance bottom left.
					struct aiStone *victim = getEnemyNeighbour(world, curStone,
							topRight);
					if (victim && stoneCanHitVictim(world, victim, topRight)) {
						/*child8 =
						 hitStone(world, curStone, victim, getTopRight(victim));

						 appendChild(world,
						 &child8);
						 depthSearch(depth - 1,
						 &child8);*/
					}
				}
			}

			curStone++;
		}
	}
}

/**
 * converts struct stone to aiStone to remove pointers.
 */
struct aiStone stoneToAiStone(struct stone src) {
	struct aiStone retValue;

	retValue.alive = src.alive;
	retValue.king = src.king;
	retValue.position = src._player->position;
	retValue.field.col = src._field->col;
	retValue.field.row = src._field->row;

	return retValue;
}

/**
 * Function initializes the minimax Depth search.
 */
struct aiMove computerStart(struct player players[]) {

	int i;

	struct board world;

	// Create a new world.
	world.player = 1;
	world.nodeValue = -1;
	world.moveCount = 0;

	for (i = 0; i < 12; i++) {
		world.botStones[i].alive = players[0].stones[i].alive;
		world.botStones[i].king = players[0].stones[i].king;
		world.botStones[i].position = 0;
		world.botStones[i].field.col = players[0].stones[i]._field->col;
		world.botStones[i].field.row = players[0].stones[i]._field->row;

		world.topStones[i].alive = players[1].stones[i].alive;
		world.topStones[i].king = players[1].stones[i].king;
		world.topStones[i].position = 1;
		world.topStones[i].field.col = players[1].stones[i]._field->col;
		world.topStones[i].field.row = players[1].stones[i]._field->row;
	}

	// create a search tree via depth search.
	depthSearch(1, &world);

	// Decide what move the computer will do.

	return world.moves[0];
}
