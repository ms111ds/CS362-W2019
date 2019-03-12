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
    char *  curFile = "cardtest4.c";
    int     numPlayers = 2;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     numErr = 0; // number of errors found
    int     i;
    int     j;
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

    // tests 
    state->hand[0][0] = great_hall;
    state->deck[0][4] = embargo;
    memcpy(originalState, state, sizeof(struct gameState));

    for (j = 0; j < 2; j++)
    {
        // test for when when no cards are left in deck
        if (j == 1)
        {
            for (i = 0; i < 5; i++)
                originalState->discard[0][i] = originalState->deck[0][i];
            originalState->discardCount[0] = 5;
            originalState->deckCount[0] = 0;
            memcpy(state, originalState, sizeof(struct gameState));
        }

        retVal = cardEffect(great_hall, -1, -1, -1, state, 0, &bonus);

        // test return value   
        if (retVal != 0)
            errMsg("great_hall encountered an error. Return not 0.", curFile, &numErr);

        // test hand size
        if (state->handCount[0] != originalState->handCount[0])
        {
            errMsg("great_hall produced incorrect hand size.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", originalState->handCount[0] - 1, state->handCount[0]);
        }

        // ensure correct cards in hand
        for (i = 0; i < originalState->handCount[0]; i++)
        {
            if (i == 0)
            {
                if (j == 0) // for j == 1 a random card is put in
                    retVal = originalState->deck[0][4];
            }
            else 
                retVal = originalState->hand[0][i];

            if ((j != 1 || i != 0) && retVal != state->hand[0][i])
            {
                errMsg("great_hall produced incorrect card in hand.", curFile, &numErr);
                printf("\tshould be %i, produced %i.\n", retVal, state->hand[0][i]);
            }
        }

        // test deck size
        if (j == 0)
        {
            if (state->deckCount[0] != originalState->deckCount[0] - 1)
            {
                errMsg("great_hall produced incorrect deck size.", curFile, &numErr);
                printf("\tshould be %i, produced %i.\n", originalState->deckCount[0] - 1, state->deckCount[0]);
            }
        }
        else
        {
            if (state->deckCount[0] != 4)
            {
                errMsg("great_hall produced incorrect deck size.", curFile, &numErr);
                printf("\tshould be %i, produced %i.\n", 4, state->deckCount[0]);
            }

        }

        // ensure correct cards in deck 
        for (i = 0; i < originalState->deckCount[0] - 1; i++)
        {
            if (j == 0 && originalState->deck[0][i] != state->deck[0][i])
            {
                errMsg("great_hall produced incorrect card in deck.", curFile, &numErr);
                printf("\tshould be %i, produced %i.\n", originalState->deck[0][i], state->deck[0][i]);
            }
        }

        // test count of played cards
        if (state->playedCardCount != originalState->playedCardCount + 1)
        {
            errMsg("great_hall produced incorrect playedCards amount.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", originalState->playedCardCount + 1, state->playedCardCount);
        }

        // see if the played card is great_hall 
        if (state->playedCards[0] != great_hall)
        {
            errMsg("great_hall did not insert itself into the played cards pile.", curFile, &numErr);
            printf("\tShould have inserted %i, actually inserted %i.\n", great_hall, state->playedCards[0]);
        }

        // test discard pile size
        if (state->discardCount[0] != 0)
        {
            errMsg("great_hall produced incorrect discard pile size.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", 0, state->discardCount[0]);
        }

        // test number of actions
        if (state->numActions != originalState->numActions + 1)
        {
            errMsg("great_hall produced an incorrect number of actions.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", originalState->numActions + 1, state->numActions);
        }
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
