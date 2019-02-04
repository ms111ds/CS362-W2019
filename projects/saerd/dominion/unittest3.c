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
    char *  curFile = "unittest3.c";
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

    // tests on player parameter values with a card known to be in the deck 

    // test estate count on players in game 
    for (i = 0; i < numPlayers; i++)
    {
        retVal = fullDeckCount(i, estate, state);
        if (retVal != 3)
        {
            errMsg("fullDeckCount failed for valid player value and valid card value.", curFile, &numErr);
            printf("\t player %i returned value %i estates, should be 3\n", i+1, retVal);
        }
    }
    // test estate count on players in game not in game 
    for (i = numPlayers; i < MAX_PLAYERS; i++)
    {
        retVal = fullDeckCount(i, estate, state);
        if (retVal != 0)
        {
            errMsg("fullDeckCount failed for person not in game with a valid card value.", curFile, &numErr);
            printf("\t player %i returned value %i estates, should be 0\n", i+1, retVal);
        }
    }
    // test for player number larger than MAX_PLAYERS
     retVal = fullDeckCount(401, estate, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for invalid person (large value) with a valid card value.", curFile, &numErr);
        printf("\t returned value %i estates, should be 0\n", retVal);
    }   
    // test for negative player number 
    retVal = fullDeckCount(-1, estate, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for invalid person (negative value) with a valid card value.", curFile, &numErr);
        printf("\t returned value %i estates, should be 0\n", retVal);
    }

    
    // tests on card values with a vaild player

    // test for a vaild card not in deck
    retVal = fullDeckCount(0, curse, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for valid person with a valid card not in deck.", curFile, &numErr);
        printf("\t returned value %i curses, should be 0\n", retVal);
    }
    // test for an invalid card (large card number)
    retVal = fullDeckCount(0, 401, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for valid person with an invalid (large number) card.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test for an invalid card (negative number)
    retVal = fullDeckCount(0, -1, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for valid person with an invalid (negative value) card.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }

    //tests on on invlid players with invalid cards

    // test for negative person and card values 
    retVal = fullDeckCount(-1, -1, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for invalid (negative value) person with an invalid (negative value) card.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test for negative person and large card values 
    retVal = fullDeckCount(-1, 401, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for invalid (negative value) person with an invalid (large value) card.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test for large person value and negative card values 
    retVal = fullDeckCount(401, -1, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for invalid (large value) person with an invalid (negative value) card.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }
    // test for large person value and large card values 
    retVal = fullDeckCount(401, 401, state);
    if (retVal != 0)
    {
        errMsg("fullDeckCount failed for invalid (large value) person with an invalid (large value) card.", curFile, &numErr);
        printf("\t returned value %i, should be 0\n", retVal);
    }

    //all tests passed or some failed
    if (numErr == 0)
        printf("\n%s: ALL TESTS PASSED!\n", curFile);
    else
        printf("\n%s: failed %i tests!\n", curFile, numErr);

    free(state);
    return 0;
}
