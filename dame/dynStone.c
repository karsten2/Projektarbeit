#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynStone.h"
#include "globals.h"

void stoneArray_init(dynStoneArray *stoneArray) {
	stoneArray->size = 0;
	stoneArray->capacity = INITIAL_CAPACITY;
	stoneArray->stone = malloc(sizeof(stone) * stoneArray->capacity);
}

void stoneArray_append(dynStoneArray *stoneArray, struct stone *stone) {
	stoneArray_double_capacity(stoneArray);
	stoneArray->stone[stoneArray->size++] = *stone;
}

struct stone * stoneArray_get(dynStoneArray *stoneArray, int index) {
	if (index >= stoneArray->size || index < 0) {
		exit(1);
	}
	return &(stoneArray->stone[index]);
}

void stoneArray_set(dynStoneArray *stoneArray, struct stone *value, int index) {
	while (index >= stoneArray->size) {
		stoneArray_append(stoneArray, NULL);
	}

	stoneArray->stone[index] = *value;
}

void stoneArray_double_capacity(dynStoneArray *stoneArray) {
	if (stoneArray->size >= stoneArray->capacity) {
		stoneArray->capacity += stoneArray->capacity;
		stoneArray->stone = realloc(stoneArray->stone,
				sizeof(struct stone) * stoneArray->capacity);
	}
}

void stoneArray_free(dynStoneArray *stoneArray) {
	free(stoneArray->stone);
}
