/******************************************************************
 * Name: Diego Saer
 * Class: CS 362
 * Description: Assignment 4 "sea_hag" random test generator
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

// unsigned int alarmTime = 5; // max time in seconds before timeout


/*************************************************************
 * variables critical to sea_hag 
 * Do not change!
 ************************************************************/
int maxNumPlayers = 4;              // max numPlayers
int minNumPlayers = 2;              // min numPlayers
int maxWhoseTurn = MAX_PLAYERS - 1; // max whoseTurn
int minWhoseTurn = 0;               // min whoseTurn
int maxHandCount = 500;             // max handCount
int minHandCount = 0;               // min handCount
int maxDeckCount = 500;             // max deckCount
int minDeckCount = 0;               // min deckCount
int maxDiscardCount = 500;          // max discardCount
int minDiscardCount = 0;            // min discardCount

/****************************************************************
 * randomizeAndInsert
 * Description: randomize the game state and add sea_hag to the
 * current player's hand. Returns the position of sea_hag in the
 * hand. 
 ***************************************************************/
int randomizeAndInsert(struct gameState * state)
{
    int i;
    int numCurses;
    int card = sea_hag;
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
        handWeight = rand() % maxHandCount;
        deckWeight = rand() % (maxDeckCount); // ensure deck always has 1 card
        discardWeight = rand() % maxDiscardCount;
        totalWeight = handWeight + deckWeight + discardWeight;
        totalCards = rand() % 501;
        state->handCount[i] = totalCards * handWeight/totalWeight;
        state->deckCount[i] = totalCards * deckWeight/totalWeight;
        state->discardCount[i] = totalCards - state->handCount[i] - state->deckCount[i];
    }

    // set the number of curse cards 
    numCurses = rand() % ((state->numPlayers - 1) * 10 + 1);
    state->supplyCount[curse] = numCurses;

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
void createExpectedState(struct gameState * expectedState)
{
    int i;
    int curPlayer = expectedState->whoseTurn;

    for(i = 0; i < expectedState->numPlayers; i++)
    {
        if (i != curPlayer)
        {
            // discard top card of deck if able to
            if (expectedState->deckCount[i] > 0)
            {
                expectedState->discard[i][expectedState->discardCount[i]] =
                     expectedState->deck[i][expectedState->deckCount[i] - 1];
                expectedState->discardCount[i]++;
                expectedState->deckCount[i]--;
            }
            // add curse card to the deck if there are some still in supply
            if (expectedState->supplyCount[curse] > 0)
            {
                expectedState->deck[i][expectedState->deckCount[i]] = curse;
                expectedState->deckCount[i]++;
                expectedState->supplyCount[curse]--;
            }
        }
    }

    return;
}



/****************************************************************
 * statePrint
 * Description:prints game state information.
 ***************************************************************/
void statePrint(struct gameState * state, char * stateName)
{
    int i;
    int numPlayers = state->numPlayers;
    int whoseTurn = state->whoseTurn;
    

    printf("......%s state values......\n", stateName);
    printf("current turn: player %i\n", whoseTurn);
    printf("numPlayers: %i\n", numPlayers);
    for(i = 0; i < numPlayers; i++)
    {
        printf("player %i\n", i);
        printf("\thandCount: %i\n", state->handCount[i]);
        printf("\tdeckCount: %i\n", state->deckCount[i]);
        printf("\tdiscardCount: %i\n", state->discardCount[i]);
    }
}

/****************************************************************
 * stateOracle
 * Description: compare two game states. Return 0 if no errors
 * are found or return -1 and print error message on first error
 * found.
 ***************************************************************/
int stateOracle(struct gameState * originalState, struct gameState * expectedState, struct gameState * postState)
{
    int retVal;

    // compare the expected game state to the post function game state
    if( memcmp(expectedState, postState, sizeof(struct gameState)) == 0)
        retVal = 0;
    else
    {
        // if the states are different, print the critical variables
        printf("------------------------------------------------------\n");
        printf("Original Number of Curses in supply: %i\n", originalState->supplyCount[curse]);
        printf("Post Number of Curses in supply: %i\n", postState->supplyCount[curse]);
        printf("Expected Number of Curses in supply: %i\n", expectedState->supplyCount[curse]);
        statePrint(originalState, "original");
        statePrint(postState, "post");
        statePrint(expectedState, "expected");


        retVal = 1;
    }

    return retVal;

}

// /****************************************************************
//  * alarmCatch 
//  * Description: catches sigalrm.
//  ***************************************************************/
// void alarmCatch(int sig)
// {
//     printf("sea_hag timed out\n");
//     exit(0);
// }

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
//     signal(SIGALRM, alarmCatch);
//     alarm(alarmTime);

    for (i = 0; i < iterations; i++)
    {
        // printf("%i: ", i);
        cardPos = randomizeAndInsert(&originalState);
        // printf("cardPos complete, ");
        memcpy(&expectedState, &originalState, sizeof(struct gameState));
        memcpy(&postState, &originalState, sizeof(struct gameState)); 
        // printf("new card made, ");
        cardEffect(sea_hag, 0, 0, 0, &postState, cardPos, &bonus);
        // printf("sea_hag run, ");
        createExpectedState(&expectedState);
        // printf("expected state obtained, ");
        bugsFound += stateOracle(&originalState, &expectedState, &postState);
        // printf("oracle run\n");
    }

    if (bugsFound == 0)
        printf("Sea_hag Test: No Bugs Found!\n");
    else
        printf("Sea_hag Test: %i Bugs Found :(\n", bugsFound);

    return 0;
}
