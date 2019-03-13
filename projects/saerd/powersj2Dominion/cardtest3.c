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
    char *  curFile = "cardtest3.c";
    int     numPlayers = 4;    
    int     k[10] = {adventurer, gardens, embargo, village, great_hall, mine, cutpurse,
            sea_hag, tribute, smithy}; // kingdom cards
    int     randomSeed = 1;
    struct  gameState * state;

    // testing variables
    int     retVal; // return value of function
    int     numErr = 0; // number of errors found
    int     i;
    struct gameState * originalState;
    int     bonus = 0;
    int     originalHandSize = 5;
    int     originalDeckSize = 5;
    int     curPlayer;
    int     origVal;
    int     fxnVal;

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
    state->hand[0][0] = sea_hag;
    for (i = 0; i < 5; i++) // player 3 will have no cards in his deck
        state->discard[2][i] = state->deck[2][i];
    state->discardCount[2] = 5;
    state->deckCount[2] = 0;
    retVal = cardEffect(sea_hag, -1, -1, -1, state, 0, &bonus);


    // test return value   
    if (retVal != 0)
        errMsg("sea_hag encountered an error. Return not 0.", curFile, &numErr);

    // player tests
    for (curPlayer = 0; curPlayer < numPlayers; curPlayer++)
    {        
        // test hand size 
        if (curPlayer == 0)
        {
            if (state->handCount[curPlayer] != originalState->handCount[curPlayer] - 1)
            {
                errMsg("sea_hag produced incorrect hand size.", curFile, &numErr);
                printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, originalState->handCount[curPlayer] - 1 , state->handCount[curPlayer]);
            }
        }
        else
        {
            if (state->handCount[curPlayer] != originalState->handCount[curPlayer])
            {
                errMsg("sea_hag produced incorrect hand size.", curFile, &numErr);
                printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, originalState->handCount[curPlayer], state->handCount[curPlayer]);
            }
        }

        // ensure correct cards in hand
        if (curPlayer == 0)
        {        
            for (i = 0; i < originalState->handCount[curPlayer] - 1; i++)
            {
                if (i == 0)
                    retVal = originalState->hand[curPlayer][4];
                else
                    retVal = originalState->hand[curPlayer][i];

                if (retVal != state->hand[curPlayer][i])
                {
                    errMsg("sea_hag produced incorrect card in hand.", curFile, &numErr);
                    printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, retVal, state->hand[curPlayer][i]);
                }
            }
        }
        else
        {
            for (i = 0; i < originalState->handCount[curPlayer] + 1; i++)
            {
                if (i == 5)
                    retVal = curse;
                else
                    retVal = originalState->hand[curPlayer][i];

                if (retVal != state->hand[curPlayer][i])
                {
                    errMsg("sea_hag produced incorrect card in hand.", curFile, &numErr);
                    printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, retVal, state->hand[curPlayer][i]);
                }
            }
        }

        // test deck size
        if (state->deckCount[curPlayer] != originalState->deckCount[curPlayer])
        {
            errMsg("sea_hag produced incorrect deck size.", curFile, &numErr);
            printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, originalState->deckCount[curPlayer], state->deckCount[curPlayer]);
        }
        
        // ensure correct cards in deck 
        for (i = 0; i < originalState->deckCount[curPlayer]; i++)
        {
            if (curPlayer != 3 && (curPlayer == 0 || i < 4))
            {        
                if (originalState->deck[curPlayer][i] != state->deck[curPlayer][i])
                {
                    errMsg("sea_hag produced incorrect card in deck.", curFile, &numErr);
                    printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, originalState->deck[curPlayer][i], state->deck[curPlayer][i]);
                }
            }
            else if (i == 5)
            {        
                if (state->deck[curPlayer][i] == curse)
                {
                    errMsg("sea_hag produced incorrect card in deck.", curFile, &numErr);
                    printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, curse, state->deck[curPlayer][i]);
                }
            }
        }

        // test discard pile size
        if (curPlayer == 0)
        {
            if (state->discardCount[curPlayer] != originalState->discardCount[curPlayer])
            {
                errMsg("sea_hag produced incorrect discard pile size.", curFile, &numErr);
                printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, originalState->discardCount[curPlayer], state->discardCount[curPlayer]);
            }
        }
        else
        {
            if (state->discardCount[curPlayer] != 1)
            {
                errMsg("sea_hag produced incorrect discard pile size.", curFile, &numErr);
                printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, 1, state->discardCount[curPlayer]);
            }
        }
        
        // ensure correct cards in discard pile 
        if (curPlayer != 0 && curPlayer != 2)
        { 
            origVal = originalState->deck[curPlayer][originalState->deckCount[curPlayer] - 1];
            fxnVal = state->discard[curPlayer][state->discardCount[curPlayer] - 1];
            if (origVal != fxnVal)
            {
                errMsg("sea_hag produced incorrect card in discard pile.", curFile, &numErr);
                printf("\tplayer %i: should be %i, produced %i.\n", curPlayer + 1, origVal, fxnVal);
            }
        }
        
        // test count of played cards
        if (curPlayer == 0 && state->playedCardCount != originalState->playedCardCount + 1)
        {
            errMsg("sea_hag produced incorrect playedCards amount.", curFile, &numErr);
            printf("\tshould be %i, produced %i.\n", originalState->playedCardCount + 1, state->playedCardCount);
        }

        // see if the played card is sea hag 
        if (curPlayer == 0 && state->playedCards[0] != sea_hag)
        {
            errMsg("sea_hag did not insert itself into the played cards pile.", curFile, &numErr);
            printf("\tShould have inserted %i.\n", sea_hag);
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
