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
    char *  curFile = "cardtest1.c";
    int     numPlayers = 2;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     numErr = 0; // number of errors found
    int     i;
    struct gameState * originalState;
    int    bonus = 0;
    int     originalHandSize = 5;
    int     originalDeckSize = 5;

    // initialize game
    state = newGame();
    retVal = initializeGame(numPlayers, k, randomSeed, state); 
    if (retVal != 0)
    {
        errMsg("game failed to initialize", curFile, &numErr);
        return -1;     
    }
    if (originalHandSize != state->handCount[0])
    {
        errMsg("check original Hand Size initialize", curFile, &numErr);
        return -1;     
    }
    if (originalDeckSize != state->deckCount[0])
    {
        errMsg("check original Deck Size initialize", curFile, &numErr);
        return -1;     
    }
    originalState = newGame();
    memcpy(originalState, state, sizeof(struct gameState));

    // tests 
    state->hand[0][0] = smithy;
    retVal = cardEffect(smithy, -1, -1, -1, state, 0, &bonus);

    // test return value   
    if (retVal != 0)
        errMsg("smithy encountered an error. Return not 0.", curFile, &numErr);
    // test hand size
    if (state->handCount[0] != originalState->handCount[0] + 2)
    {
        errMsg("smithy produced incorrect hand size.", curFile, &numErr);
        printf("\tshould be %i, produced %i.\n", originalState->handCount[0] + 2 , state->handCount[0]);
    }
    // ensure correct cards in hand
    for (i = 0; i < originalState->handCount[0] + 2; i++)
    {
        if (i == 0)
            retVal = originalState->deck[0][2];
        else if (i < 5)
            retVal = originalState->hand[0][i];
        else
            retVal = originalState->deck[0][i - 2];

        if (retVal != state->hand[0][i])
        {
            errMsg("smithy produced incorrect card in hand.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", retVal, state->hand[0][i]);
        }
    }
    // test deck size
    if (state->deckCount[0] != originalState->deckCount[0] - 3)
    {
        errMsg("smithy produced incorrect deck size.", curFile, &numErr);
        printf("\tshould be %i, produced %i.\n", originalState->deckCount[0] - 3, state->deckCount[0]);
    }
    // ensure correct cards in deck 
    for (i = 0; i < originalState->deckCount[0] - 3; i++)
    {
        if (originalState->deck[0][i] != state->deck[0][i])
        {
            errMsg("smithy produced incorrect card in deck.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", originalState->deck[0][i], state->deck[0][i]);
        }
    }
    // test count of played cards
    if (state->playedCardCount != originalState->playedCardCount + 1)
    {
        errMsg("smithy produced incorrect playedCards amount.", curFile, &numErr);
        printf("\tshould be %i, produced %i.\n", originalState->playedCardCount + 1, state->playedCardCount);
    }
    // see if the played card is smithy 
    if (state->playedCards[0] != smithy)
    {
        errMsg("smithy did not insert itself into the played cards pile.", curFile, &numErr);
        printf("\tShould have inserted %i, actually inserted %i.\n", smithy, state->playedCards[0]);
    }


    //all tests passed or some failed
    if (numErr == 0)
        printf("\n%s: ALL TESTS PASSED!\n", curFile);
    else
        printf("\n%s: failed %i tests!\n", curFile, numErr);

    free(originalState);
    free(state);
    return 0;
}
