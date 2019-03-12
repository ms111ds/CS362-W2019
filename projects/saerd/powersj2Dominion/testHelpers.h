#ifndef _TESTHELPERS_H
#define _TESTHELPERS_H

#include "dominion.h"

#define TEST_DEBUG 0

void printGameState(int player, struct gameState *state);
int testAssert(int test, char* testName);

#endif