/******************************************************************
 * Name: Diego Saer
 * Class: CS 362
 * Description: Assignment 4 "smithy" random test generator
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <time.h>


/*************************************************************
 * variables critical to smithy
 * Do not Change!
 ************************************************************/
int maxNumPlayers = 4;              // max numPlayers
int minNumPlayers = 2;              // min numPlayers
int maxWhoseTurn = MAX_PLAYERS - 1;  // max whoseTurn
int minWhoseTurn = 0;               // min whoseTurn
int maxHandCount = 500;             // max handCount
int minHandCount = 0;               // min handCount
int maxDeckCount = 500;             // max deckCount
int minDeckCount = 0;               // min deckCount
int maxDiscardCount = 500;          // max discardCount
int minDiscardCount = 0;            // min discardCount
int maxPlayedCardCount = 500;       // max playedCardCount
int minPlayedCardCount = 0;

/****************************************************************
 * randomizeAndInsert
 * Description: randomize the game state and add smithy to the
 * current player's hand. Returns the position of smithy in the
 * hand.
 ***************************************************************/
int randomizeAndInsert(struct gameState * state)
{
    int i;
    int card = smithy;
    int handPos;
    int handWeight;
    int deckWeight;
    int discardWeight;
    int totalWeight;
    int totalCards;

    // randomize game state
    for (i = 0; i < sizeof(struct gameState); i++)
        ((char *)state)[i] = rand() % 0x100;

    // ensure critical variables are correct 
    state->numPlayers = minNumPlayers + rand() % (maxNumPlayers - minNumPlayers + 1);
    state->whoseTurn = rand() % state->numPlayers;
    for (i = 0; i < state->numPlayers; i++)
    {
        handWeight = rand() % 500;
        deckWeight = rand() % 500;
        discardWeight = rand() % 500;
        totalWeight = handWeight + deckWeight + discardWeight;
        totalCards = rand() % 501;
        state->handCount[i] = totalCards * handWeight/totalWeight;
        state->deckCount[i] = totalCards * deckWeight/totalWeight;
        state->discardCount[i] = totalCards - state->handCount[i] - state->deckCount[i];
    }
    state->playedCardCount = minPlayedCardCount + rand() % (maxPlayedCardCount - minPlayedCardCount + 1);

    // insert card into the player's hand and return the card's
    // position in the hand
    if (state->handCount[state->whoseTurn] == 0)
        state->handCount[state->whoseTurn] = 1;
    handPos = rand() % state->handCount[state->whoseTurn];
    state->hand[state->whoseTurn][handPos] = card;
    return handPos;
}        

/****************************************************************
 * createExpectedState
 * Description: fill out the information for the expected game
 *      state.
 ***************************************************************/
void createExpectedState(struct gameState * expectedState, struct gameState * postState, int handPos)
{
    int i;
    int deckSwitch = 0; // determines if the discard has been shuffled into the deck
    int curPlayer = expectedState->whoseTurn;
    int replaceIndex;

    for (i = 0; i < 3; i++)
    {
        
        if (expectedState->deckCount[curPlayer] > 0 && deckSwitch == 0)
        {
            // check if max hand size reached 
            if (expectedState->handCount[curPlayer] >= maxHandCount)
            {
                printf("createExpectedState: adding card beyond max\n");
                break;
            }

            // update hand with card from deck
            expectedState->hand[curPlayer][expectedState->handCount[curPlayer]]
                = expectedState->deck[curPlayer][expectedState->deckCount[curPlayer] - 1]; 
            // update hand and deck counts
            expectedState->handCount[curPlayer]++;
            expectedState->deckCount[curPlayer]--;
        }

        else if(expectedState->discardCount[curPlayer] > 0 || (expectedState->deckCount[curPlayer] > 0 && deckSwitch == 1))
        {
            // shuffle discard into deck
            if (deckSwitch == 0)
            {
                // copy over shuffled deck from post state
                memcpy(expectedState->deck[curPlayer], postState->deck[curPlayer], sizeof(int) * expectedState->discardCount[curPlayer]);
                // copy over discard pile from post state
                memcpy(expectedState->discard[curPlayer], postState->discard[curPlayer], sizeof(int) * expectedState->discardCount[curPlayer]);
                // update the deck count, discard count 
                expectedState->deckCount[curPlayer] = expectedState->discardCount[curPlayer]; 
                expectedState->discardCount[curPlayer] = 0;
                // update the card in smithy's hand position, the deck count, and set the deck switch
                expectedState->hand[curPlayer][handPos] = postState->hand[curPlayer][handPos];
                expectedState->deckCount[curPlayer]--;
                deckSwitch = 1;
            }
            else
            {

                // check if max hand size reached
                if (expectedState->handCount[curPlayer] >= maxHandCount)
                {
                    printf("createExpectedState: adding card beyond max\n");
                    break;
                }

                // update hand with corresponding card from post function deck
                if (postState->handCount[curPlayer] < 3)
                    replaceIndex = i;
                else
                    replaceIndex = postState->handCount[curPlayer] - 3 + i;
                expectedState->hand[curPlayer][expectedState->handCount[curPlayer]]
                    = postState->hand[curPlayer][replaceIndex];
                // update hand and deck counts
                expectedState->handCount[curPlayer]++;
                expectedState->deckCount[curPlayer]--;
            }
        }
    }

    // remove smithy from hand if not done already (discard not reshuffled into deck)
    if (deckSwitch == 0)
    {
        expectedState->hand[curPlayer][handPos] = expectedState->hand[curPlayer][expectedState->handCount[curPlayer] - 1];
        expectedState->hand[curPlayer][expectedState->handCount[curPlayer] - 1] = -1;
        expectedState->handCount[curPlayer]--;
    }
    // if the discard was reshuffled into the deck the last card would have been moved
    // to the place where smithy was. It's previous place needs to be set equal to -1
    else
        expectedState->hand[curPlayer][expectedState->handCount[curPlayer]] = -1;

    expectedState->playedCards[expectedState->playedCardCount] = smithy;
    expectedState->playedCardCount++;

    return;
}



/****************************************************************
 * statePrint
 * Description:prints game state information.
 ***************************************************************/
void statePrint(struct gameState * state, char * stateName)
{
    int numPlayers = state->numPlayers;
    int whoseTurn = state->whoseTurn;


    printf("......%s state values......\n", stateName);
    printf("numPlayers: %i\n", numPlayers);
    printf("whoseTurn: %i\n", whoseTurn);
    printf("handCount: %i\n", state->handCount[whoseTurn]);
    printf("deckCount: %i\n", state->deckCount[whoseTurn]);
    printf("discardCount: %i\n", state->discardCount[whoseTurn]);
    printf("playedCardCount: %i\n", state->playedCardCount);
}

/****************************************************************
 * stateOracle
 * Description: compare two game states. Return 0 if no errors
 * are found or return -1 and print error message on first error
 * found.
 ***************************************************************/
int stateOracle(struct gameState * originalState, struct gameState * expectedState, struct gameState * postState)
{
    int i;    
    int retVal;

    // compare the expected game state to the post function game state
    if( memcmp(expectedState, postState, sizeof(struct gameState)) == 0)
        retVal = 0;
    else
    {
        // if the states are different, print the critical variables
        printf("------------------------------------------------------\n");
        statePrint(originalState, "original");
        statePrint(postState, "post");
        statePrint(expectedState, "expected");

        // search through the hand, deck, and discard pile. Find where the
        // expected and post function game state are different.
        for (i = 0; i < maxHandCount; i++)
        {
            if (expectedState->hand[expectedState->whoseTurn][i] != postState->hand[postState->whoseTurn][i])
            {
                printf("hand[%i]: expected %i, ", i, expectedState->hand[expectedState->whoseTurn][i]);
                printf("post %i, ", postState->hand[postState->whoseTurn][i]); 
                printf("original %i\n", originalState->hand[originalState->whoseTurn][i]); 
            }
        } 
        for (i = 0; i < maxDeckCount; i++)
        {
            if (expectedState->deck[expectedState->whoseTurn][i] != postState->deck[postState->whoseTurn][i])
            {
                printf("deck[%i]: expected %i, ", i, expectedState->deck[expectedState->whoseTurn][i]);
                printf("post %i, ", postState->deck[postState->whoseTurn][i]); 
                printf("original %i\n", originalState->deck[originalState->whoseTurn][i]); 
            }
        } 
        for (i = 0; i < maxDiscardCount; i++)
        {
            if (expectedState->discard[expectedState->whoseTurn][i] != postState->discard[postState->whoseTurn][i])
            {
                printf("discard[%i]: expected %i, ", i, expectedState->discard[expectedState->whoseTurn][i]);
                printf("post %i, ", postState->discard[postState->whoseTurn][i]); 
                printf("original %i\n", originalState->discard[originalState->whoseTurn][i]); 
            }
        } 

        retVal = 1;
    }

    return retVal;

}

int main(int argc, char * argv[])
{
    int i;
    int iterations;
    int cardPos;
    int bonus = 0;
    int bugsFound = 0;
    struct gameState originalState;
    struct gameState expectedState;
    struct gameState postState;

    iterations = atoi(argv[1]);

    srand(time(0));

    for (i = 0; i < iterations; i++)
    {
        // printf("%i: ", i);
        cardPos = randomizeAndInsert(&originalState);
        memcpy(&expectedState, &originalState, sizeof(struct gameState));
        memcpy(&postState, &originalState, sizeof(struct gameState)); 
        // printf("new card made, ");
        cardEffect(smithy, 0, 0, 0, &postState, cardPos, &bonus);
        // printf("smithy run, ");
        createExpectedState(&expectedState, &postState, cardPos);
        // printf("expected state obtained, ");
        bugsFound += stateOracle(&originalState, &expectedState, &postState);
        // printf("oracle run\n");
    }

    if (bugsFound == 0)
        printf("Smithy Test: No Bugs Found!\n");
    else
        printf("Smithy Test: %i Bugs Found :(\n", bugsFound);

    return 0;
}
