/**
 * Jake Powers 
 * This is a collection of functions used in the unit and randoms testAssert
 * for the dominion code. 
 * 2.23.19
*/


#include "testHelpers.h"
#include <stdlib.h>
#include <stdio.h>

void printGameState(int player, struct gameState *state){
    printf(
  "numPlayers: %d\n" 
  "whoseTurn: %d\n"
  "phase: %d\n"
  "numActions: %d\n" 
  "coins: %d\n"
  "numBuys: %d\n" 
  "handCount: %d\n"
  "deckCount: %d\n"
  "discardCount: %d\n"
  "playedCardCount: %d\n\n",state->numPlayers,state->whoseTurn,state->phase,state->numActions,state->coins,
  state->numBuys,state->handCount[player],state->deckCount[player],state->discardCount[player], state->playedCardCount);
}

int testAssert(int test, char* testName){
    if(!test){
       if(TEST_DEBUG) printf("%s Assertion failed:(x)\n",testName);
            return 0;
    }else{
       if(TEST_DEBUG) printf("%s Pass: (\xE2\x9C\x93)\n", testName);
        return 1;
    }
}