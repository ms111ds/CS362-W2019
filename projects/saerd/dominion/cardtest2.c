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
#include <signal.h>
#include <unistd.h>

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

/*****************************************************************
 * exitFxn
 * Description: exits the program.
 * Found on: https://stackoverflow.com/questions/15241013/how-to-exit-main-function-after-executing-for-specific-time-in-c 
 ****************************************************************/
void exitFxn(int sig)
{
    printf("Test timed out. Exiting\n");
    exit(0);
}


int main()
{
    // game setup variables
    char *  curFile = "cardtest2.c";
    int     numPlayers = 2;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     retVal2;
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
    memcpy(originalState, state, sizeof(struct gameState));

    // tests 
    signal(SIGALRM, exitFxn);
    alarm(5);
    j = 0;
    while (j < 2)
    {
        state->hand[0][0] = adventurer;
        retVal = cardEffect(adventurer, -1, -1, -1, state, 0, &bonus);

        // test return value   
        if (retVal != 0)
            errMsg("adventurer encountered an error. Return not 0.", curFile, &numErr);

        // test hand size
        if (state->handCount[0] != originalState->handCount[0] + 1)
        {
            errMsg("adventurer produced incorrect hand size.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", originalState->handCount[0] + 1 , state->handCount[0]);
        }

        // ensure correct cards in hand
        for (i = 0; i < originalState->handCount[0] + 1; i++)
        {
            if (i == 0 || i >= 5)
            {
                if (state->hand[0][i] != copper && state->hand[0][i] != silver && state->hand[0][i] != gold)
                {
                    errMsg("adventurer produced incorrect card in hand.", curFile, &numErr);
                    printf("\tshould be %i, %i, or %i, produced %i.\n", copper, silver, gold, state->hand[0][i]);
                }
            }
            else 
            {
                retVal = originalState->hand[0][i];
                if (retVal != state->hand[0][i])
                {
                    errMsg("adventurer produced incorrect card in hand.", curFile, &numErr);
                    printf("\tshould be %i, produced %i.\n", retVal, state->hand[0][i]);
                }
            }
        }

        // test deck size
        retVal = originalState->deckCount[0] - state->deckCount[0];
        retVal2 = state->handCount[0] - originalState->handCount[0]
                  + state->playedCardCount - originalState->playedCardCount
                  + state->discardCount[0] - originalState->discardCount[0];
        if (retVal != retVal2)
        {
            errMsg("Adventurer produced incorrect deck size.", curFile, &numErr);
            printf("\tChange in deck size produced %i, it is not equal to the difference in hand count, played card count, and discard count produced, which is %i.\n", originalState->deckCount[0] - 3, state->deckCount[0]);
        }


        // see if the played card is adventurer 
        if (state->playedCards[0] != adventurer)
        {
            errMsg("adventurer did not insert itself into the played cards pile.", curFile, &numErr);
            printf("\tShould have inserted %i, actually inserted %i.\n", adventurer, state->playedCards[0]);
        }
        
        // set up for another round of testing, this time there aren't 
        // enough cards to draw in the deck
        if (j == 0)
        {
            memcpy(state, originalState, sizeof(struct gameState));
            for (i = 0; i < 4; i++)
            {
                state->discard[0][i] = state->hand[0][i];
                state->hand[0][i] = state->deck[0][i];
            }
            state->deckCount[0] = 1;
            state->discardCount[0] = 4;
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
