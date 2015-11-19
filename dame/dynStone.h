#include "globals.h"

#define INITIAL_CAPACITY 25

void stoneArray_init(dynStoneArray *stoneArray);
void stoneArray_append(dynStoneArray *stoneArray, struct stone *stone);
struct stone * stoneArray_get(dynStoneArray *stoneArray, int index);
void stoneArray_set(dynStoneArray *stoneArray, struct stone *value, int index);
void stoneArray_double_capacity(dynStoneArray *stoneArray);
void stoneArray_free(dynStoneArray *stoneArray);
