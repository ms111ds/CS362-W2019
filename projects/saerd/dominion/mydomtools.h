#ifndef _MYDOMTOOLS_H
#define _MYDOMTOOLS_H

#include "dominion.h"

struct gameState * copyState(struct gameState * stateToCopy);
void intArrPrint(int * intArr, unsigned numElem);
void printState(struct gameState * state, int MaxMode);

#endif
