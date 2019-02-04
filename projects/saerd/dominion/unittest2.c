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
    char *  curFile = "unittest2.c";
    int     numPlayers = 2;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     numErr = 0; // number of errors found
    int     adventurerCards;
    int     gardensCards;
    int     embargoCards;
    // int     villageCards;
    int     great_hallCards;
    // int     mineCards;
    // int     cutpurseCards;
    // int     sea_hagCards;
    // int     tributeCards;
    // int     smithyCards;


    // initialize game
    state = newGame();
    retVal = initializeGame(numPlayers, k, randomSeed, state); 
    if (retVal != 0)
    {
        errMsg("game failed to initialize", curFile, &numErr);
        return -1;     
    }
    // tests with two players
    adventurerCards = 10;
    gardensCards = 8;
    embargoCards = 10;
    // villageCards = 10;
    great_hallCards = 8;
    // mineCards = 10;
    // cutpurseCards = 10;
    // sea_hagCards = 10;
    // tributeCards = 10;
    // smithyCards = 10;

    // test adventurer supply
    retVal = supplyCount(adventurer, state);
    if (retVal != adventurerCards)
    {
        errMsg("2 player adventurer supply not equal to 10.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test gardens supply
    retVal = supplyCount(gardens, state);
    if (retVal != gardensCards)
    {
        errMsg("2 player gardens supply not equal to 8.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }    
    // test embargo supply
    retVal = supplyCount(embargo, state);
    if (retVal != embargoCards)
    {        
        errMsg("2 player embargo supply not equal to 10.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test great hall supply
    retVal = supplyCount(great_hall, state);
    if (retVal != great_hallCards)
    {
        errMsg("2 player great hall supply not equal to 8.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test supply of card not in kingdom pile
    retVal = supplyCount(baron, state);
    if (retVal != -1)
    {        
        errMsg("2 player supply of card not in game not equal to -1.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test negative card value
    retVal = supplyCount(-1, state);
    if (retVal != -1)
    {
        errMsg("negative card value not handled.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test very large card value
    retVal = supplyCount(5001, state);
    if (retVal != -1) 
    {
        errMsg("large card value not handled.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }

    // tests with 3 players
    gardensCards = 12;
    great_hallCards = 12;
    numPlayers = 3;
    retVal = initializeGame(numPlayers, k, randomSeed, state); 
    if (retVal != 0)
    {
        errMsg("game failed to initialize", curFile, &numErr);
        return -1;     
    }

    // test adventurer supply
    retVal = supplyCount(adventurer, state);
    if (retVal != adventurerCards)
    {
        errMsg("3 player adventurer supply not equal to 10.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test gardens supply
    retVal = supplyCount(gardens, state);
    if (retVal != gardensCards)
    {
        errMsg("3 player gardens supply not equal to 12.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test embargo supply
    retVal = supplyCount(embargo, state);
    if (retVal != embargoCards)
    {
        errMsg("3 player embargo supply not equal to 10.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test great hall supply
    retVal = supplyCount(great_hall, state);
    if (retVal != great_hallCards)
    {
        errMsg("3 player great hall supply not equal to 12.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }
    // test supply of card not in kingdom pile
    retVal = supplyCount(baron, state);
    if (retVal != -1)
    {
        errMsg("3 player supply of card not in game not equal to -1.", curFile, &numErr);
        printf("\t returned value %i\n", retVal);
    }

    //all tests passed or some failed
    if (numErr == 0)
        printf("\n%s: ALL TESTS PASSED!\n", curFile);
    else
        printf("\n%s: failed %i tests!\n", curFile, numErr);

    free(state);
    return 0;
}
