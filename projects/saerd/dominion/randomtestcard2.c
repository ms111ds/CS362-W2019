/******************************************************************
 * Name: Diego Saer
 * Class: CS 362
 * Description: Assignment 4 "adventurer" random test generator
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

unsigned int alarmTime = 5; // max time in seconds before timeout


/*************************************************************
 * variables critical to adventurer 
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
int maxPlayedCardCount = 500;       // max playedCardCount
int minPlayedCardCount = 0;         // min playedCardCount

/****************************************************************
 * randomizeAndInsert
 * Description: randomize the game state and add adventurer to the
 * current player's hand. Returns the position of adventurer in the
 * hand. Also obtains the number of treasure cards inserted.
 ***************************************************************/
int randomizeAndInsert(struct gameState * state, int * treasureCount)
{
    int i;
    int numTreasure;
    int curTreasure;
    int deckAndDiscardSize;
    int treasureIndex;
    int card = adventurer;
    int handPos;
    int handWeight;
    int deckWeight;
    int discardWeight;
    int totalWeight;
    int totalCards;
    int curCard;

    // randomize game state
    for (i = 0; i < sizeof(struct gameState); i++)
        ((char *)state)[i] = rand() % 0x100;

    // ensure critical variables are correct 
    state->numPlayers = minNumPlayers + rand() % (maxNumPlayers - minNumPlayers + 1);
    state->whoseTurn = rand() % state->numPlayers;
    for (i = 0; i < state->numPlayers; i++)
    {
        handWeight = rand() % maxHandCount;
        deckWeight = rand() % maxDeckCount;
        discardWeight = rand() % maxDiscardCount;
        totalWeight = handWeight + deckWeight + discardWeight;
        totalCards = rand() % 501;
        state->handCount[i] = totalCards * handWeight/totalWeight;
        state->deckCount[i] = totalCards * deckWeight/totalWeight;
        state->discardCount[i] = totalCards - state->handCount[i] - state->deckCount[i];
    }
    state->playedCardCount = minPlayedCardCount + rand() % (maxPlayedCardCount - minPlayedCardCount + 1);

    // insert anywhere from 0 to 10 treasure cards into either the deck or discard
    numTreasure = rand() % 11;
    for (i = 0; i < numTreasure; i++)
    {
        curTreasure = copper + rand() % 3; 
        deckAndDiscardSize = state->deckCount[state->whoseTurn] + state->discardCount[state->whoseTurn];
        if (deckAndDiscardSize == 0)
            break;
        treasureIndex = rand() % deckAndDiscardSize;
        if (treasureIndex < state->deckCount[state->whoseTurn])
            state->deck[state->whoseTurn][treasureIndex] = curTreasure;
        else 
        {
            treasureIndex -= state->deckCount[state->whoseTurn];
            state->discard[state->whoseTurn][treasureIndex] = curTreasure;
        }
    }


    // insert card into the player's hand and return the card's
    // position in the hand
    if (state->handCount[state->whoseTurn] == 0)
        state->handCount[state->whoseTurn] = 1;
    handPos = rand() % state->handCount[state->whoseTurn];
    state->hand[state->whoseTurn][handPos] = card;

    // obtain the number of treasures
    *treasureCount = 0;
    for (i = 0; i < state->handCount[state->whoseTurn]; i++)
    {
        curCard = state->hand[state->whoseTurn][i];
        if (curCard == copper || curCard == silver || curCard == gold)
            (*treasureCount)++;
    }
    for (i = 0; i < state->discardCount[state->whoseTurn]; i++)
    {
        curCard = state->discard[state->whoseTurn][i];
        if (curCard == copper || curCard == silver || curCard == gold)
            (*treasureCount)++;
    }

    return handPos;
}        


/****************************************************************
 * min 
 * Description: returns the smaller of two integers
 ***************************************************************/
int min(int int1, int int2)
{
    int minimum;

    if (int1 < int2)
        minimum = int1;
    else
        minimum = int2;
    return minimum;
}


/****************************************************************
 * addToDiscardArray 
 * Description: adds next card in deck to the discard array
 ***************************************************************/
int addToDiscardArray(struct gameState * state, int (* discardArray)[], int * arrSize)
{
    int curPlayer = state->whoseTurn;
    int maxAllowable = 500;

    if (*arrSize >= maxAllowable)
    {
        printf("addToDiscardArray: adding more cards than max allowable of %i\n", maxAllowable);
        return -1;
    } 

    (*discardArray)[*arrSize] = state->deck[curPlayer][state->deckCount[curPlayer] - 1];
    state->deckCount[curPlayer]--;
    (*arrSize)++;

    return 0;
}


/****************************************************************
 * createExpectedState
 * Description: fill out the information for the expected game
 *      state.
 ***************************************************************/
void createExpectedState(struct gameState * expectedState, struct gameState * postState)
{
    int i;
    int j = 0;
    int treasureFound = 0;
    int toDiscard[1000];
    int numToDiscard = 0;
    int curCard;
    int deckSwitch = 0; // determines if the discard has been shuffled into the deck
    int curPlayer = expectedState->whoseTurn;
    int handSize;

     while (treasureFound < 2 && deckSwitch == 0 && j < 500)
     {

         if (expectedState->discardCount[curPlayer] == 0 && expectedState->deckCount[curPlayer] == 0)
             deckSwitch = 1;
          //printf("passed discard and expected = 0\n");
          else if (expectedState->deckCount[curPlayer] > 0)
          {
              curCard = expectedState->deck[curPlayer][expectedState->deckCount[curPlayer] - 1];
              //trasure card found
              if (curCard == copper || curCard == silver || curCard == gold)
              {
                  // check if max hand size reached 
                  if (expectedState->handCount[curPlayer] >= maxHandCount)
                  {
                      printf("createExpectedState: adding card beyond max to hand\n");
                      break;
                  }

                  // update hand with card from deck
                  expectedState->hand[curPlayer][expectedState->handCount[curPlayer]] = curCard; 
                  // update hand and deck counts. Then increase the number of treasures found
                  expectedState->handCount[curPlayer]++;
                  expectedState->deckCount[curPlayer]--;
                  treasureFound++;
              }
              // treasure card not found
              else
              {
                  if (addToDiscardArray(expectedState, &toDiscard, &numToDiscard) != 0)
                      return;
              }
          }
         //printf("passed no shuffle\n");

          else if(expectedState->discardCount[curPlayer] > 0)
          {
              // shuffle discard into deck
              
              // copy over shuffled deck from post state
              memcpy(expectedState->deck[curPlayer], postState->deck[curPlayer], sizeof(int) * expectedState->discardCount[curPlayer]);
              // copy over discard pile from post state
              memcpy(expectedState->discard[curPlayer], postState->discard[curPlayer], sizeof(int) * maxDiscardCount);
              // update the deck count, discard count 
              expectedState->deckCount[curPlayer] = postState->deckCount[curPlayer]; 
              expectedState->discardCount[curPlayer] = postState->discardCount[curPlayer];
              // get hand size 
              handSize = min(postState->handCount[curPlayer], expectedState->handCount[curPlayer] + (2 - treasureFound));
              handSize = min(handSize, maxHandCount);
              // update hand with treasure cards 
              while (expectedState->handCount[curPlayer] < handSize)
              {
                  // check if max hand size reached
                  if (expectedState->handCount[curPlayer] >= maxHandCount)
                  {
                      printf("createExpectedState: adding card beyond max to hand\n");
                      break;
                  }
                  expectedState->hand[curPlayer][expectedState->handCount[curPlayer]] =
                      postState->hand[curPlayer][expectedState->handCount[curPlayer]];
                  expectedState->handCount[curPlayer]++;
              }
              // set the deck Switch
              deckSwitch = 1;
          }
         j++;
     }

     // and add cards in discard array to discard pile
     if (deckSwitch == 0)
     {
         for (i = numToDiscard - 1; i >= 0; i--)
         {
             // check if max discard size reached
             if (expectedState->discardCount[curPlayer] >= maxDiscardCount)
             {
                 printf("createExpectedState: adding card beyond max to discard pile\n");
                 break;
             }
             expectedState->discard[curPlayer][expectedState->discardCount[curPlayer]] = toDiscard[i];
             expectedState->discardCount[curPlayer]++;    
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
int stateOracle(struct gameState * originalState, struct gameState * expectedState, struct gameState * postState, int treasureCount)
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
        printf("treasures in deck and discard: %i\n", treasureCount);
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

// /****************************************************************
//  * alarmCatch 
//  * Description: catches sigalrm.
//  ***************************************************************/
// void alarmCatch(int sig)
// {
//     printf("adventurer timed out\n");
//     exit(0);
// }

int main(int argc, char * argv[])
{
    int i;
    int iterations;
    int cardPos;
    int treasureCount = 0;
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
        cardPos = randomizeAndInsert(&originalState, &treasureCount);
        // printf("cardPos complete, ");
        memcpy(&expectedState, &originalState, sizeof(struct gameState));
        memcpy(&postState, &originalState, sizeof(struct gameState)); 
        // printf("new card made, ");
        cardEffect(adventurer, 0, 0, 0, &postState, cardPos, &bonus);
        // printf("adventurer run, ");
        createExpectedState(&expectedState, &postState);
        // printf("expected state obtained, ");
        bugsFound += stateOracle(&originalState, &expectedState, &postState, treasureCount);
        // printf("oracle run\n");
    }

    if (bugsFound == 0)
        printf("Adventurer Test: No Bugs Found!\n");
    else
        printf("Adventurer Test: %i Bugs Found :(\n", bugsFound);

    return 0;
}
