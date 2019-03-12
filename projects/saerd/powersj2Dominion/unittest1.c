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
    char *  curFile = "unittest1.c";
    int     numPlayers = 2;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     numErr = 0; // number of errors found

    // initialize game
    state = newGame();
    retVal = initializeGame(numPlayers, k, randomSeed, state); 
    if (retVal != 0)
    {
        errMsg("game failed to initialize", curFile, &numErr);
        return -1;     
    }

    // tests
    // initial value test
    if (whoseTurn(state) != 0)
        errMsg("WhoseTurn did not return 0 upon initialization", curFile, &numErr);
    // min int value test
    state->whoseTurn = -2147483648;
    if (whoseTurn(state) != -2147483648)
        errMsg("WhoseTurn did not return min int value (-2147483648)", curFile, &numErr);
    // min int value test
    state->whoseTurn = 2147483647;
    if (whoseTurn(state) != 2147483647)
        errMsg("WhoseTurn did not return max int value (2147483647)", curFile, &numErr);   
    //all tests passed or some failed
    if (numErr == 0)
        printf("\n%s: ALL TESTS PASSED!\n", curFile);
    else
        printf("\n%s: failed %i tests!\n", curFile, numErr);

    free(state);
    return 0;
}
