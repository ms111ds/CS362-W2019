/******************************************************************
 * Name: Diego Saer
 * Class: CS 362
 * Description: Assignment 3 function test for "numHandCards" fxn
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>

/*****************************************************************
 * errMsg
 * Description: Prints the error message in the parameter and 
 * increments the number of errors
 ****************************************************************/
void errMsg(char * msg, char * curFile, int * numErr)
{
    printf("%s: %s\n", curFile, msg);
    (*numErr)++;
}




int main()
{
    // game setup variables
    char *  curFile = "unittest4.c";
    int     numPlayers = 2;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     numErr = 0; // number of errors found
    int     i;

    // initialize game
    state = newGame();
    retVal = initializeGame(numPlayers, k, randomSeed, state); 
    if (retVal != 0)
    {
        errMsg("game failed to initialize", curFile, &numErr);
        return -1;     
    }

    // tests 

    // test when game just started 
     retVal = isGameOver(state);
    if (retVal != 0)
    {
        errMsg("isGameOver failed upon starting a new game.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test when supply count of all cards equal to negative value 
    for (i = 0; i < treasure_map + 1; i++)
        state->supplyCount[i] = -1;
    retVal = isGameOver(state);
    if (retVal != 0)
    {
        errMsg("isGameOver failed when all cards in supply set to -1.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test when the province supply reduced to zero 
    state->supplyCount[3] = 0;
    retVal = isGameOver(state);
    if (retVal != 1)
    {
        errMsg("isGameOver failed when the number of provices was set to 0.", curFile, &numErr);
        printf("\t returned value %i, should be 1\n", retVal);
    }
    // test when only one supply pile (not provinces) equal to zero 
    state->supplyCount[3] = -1;
    state->supplyCount[0] = 0;
    retVal = isGameOver(state);
    if (retVal != 0)
    {
        errMsg("isGameOver failed when the card count of a single supply pile (not province) set to 0.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test when two supply piles (not provinces) equal to zero 
    state->supplyCount[1] = 0;
    retVal = isGameOver(state);
    if (retVal != 0)
    {
        errMsg("isGameOver failed when the card count of two supply piles (neither provinces) set to 0.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test when three supply piles (not provinces) equal to zero 
    state->supplyCount[2] = 0;
    retVal = isGameOver(state);
    if (retVal != 1)
    {
        errMsg("isGameOver failed when the card count of three supply piles (neither provinces) set to 0.", curFile, &numErr);
        printf("\t returned value %i, should be 1\n", retVal);
    }
    // test when final three supply piles equal to zero 
    for (i = 0; i < 3; i++)
    {
        state->supplyCount[i] = -1;
        state->supplyCount[treasure_map - 2 + i] = 0;
    }
    retVal = isGameOver(state);
    if (retVal != 1)
    {
        errMsg("isGameOver failed when final the card count of the final 3 cards (salvager, sea_hag, treasure_map) set to 0. Allowing cards not in game to count.", curFile, &numErr);
        printf("\t returned value %i, should be 1\n", retVal);
    }
    // test when supply piles of three cards not in game set to zero 
    for (i = 0; i < 3; i++)
    {
        state->supplyCount[treasure_map - 2 + i] = -1;
    }
    state->supplyCount[baron] = 0;
    state->supplyCount[feast] = 0;
    state->supplyCount[steward] = 0;
    retVal = isGameOver(state);
    if (retVal != 1)
    {
        errMsg("isGameOver failed when the card count of 3 cards not in game (baron, feast, steward) set to 0. Allowing cards not in game to count.", curFile, &numErr);
        printf("\t returned value %i, should be 1\n", retVal);
    }

    //all tests passed or some failed
    if (numErr == 0)
        printf("\n%s: ALL TESTS PASSED!\n", curFile);
    else
        printf("\n%s: failed %i tests!\n", curFile, numErr);

    free(state);
    return 0;
}
