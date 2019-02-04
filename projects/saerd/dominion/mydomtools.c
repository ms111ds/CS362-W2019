/******************************************************************
 * Name: Diego Saer
 * Class: CS 362
 * Description: personal functions 
 *****************************************************************/

#include <stdio.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include "mydomtools.h"


// /****************************************************************
//  * arrCpy
//  * Description: copies elements from arrCpyFrom to arrCpyTo. The
//  * number of elements copied is numElem. The size of each data
//  * element is sizeDataType. No bounds checking
//  ****************************************************************/
// void arrCpy(void * arrCpyTo, void * arrCpyFrom, size_t sizeDataType, unsigned numElem)
// {
//     unsigned    i;
//     void *      cpyTo = arrCpyTo;
//     void *      cpyFrom = arrCpyFrom;
// 
//     for (i = 0; i < numElem; i++)
//     {
//         *cpyTo = *cpyFrom;
//         cpyTo += sizeDataType;
//         cpyFrom += sizeDataType;
//     }
// } 

/*****************************************************************
 * copyState
 * Description: Dynamically allocates memory for a new game state
 * and copies the data from an existing game state to the newly
 * allocated memory. Returns a pointer to the dynamically allocated
 * game state structure.
 * ***************************************************************/
struct gameState * copyState(struct gameState * stateToCopy)
{
    struct gameState *  newState;

    newState = newGame();
    if (newState == NULL)
        return newState;

    memcpy(newState, stateToCopy, sizeof(struct gameState));


    return newState;
}


/****************************************************************
 * intArrPrint
 * Description: Prints all integers in an array on a single line.
 * No bounds checking.
 ****************************************************************/
void intArrPrint(int * intArr, unsigned numElem)
{
    unsigned    i;

    for (i = 0; i < numElem; i++)
    {
        printf("%i, ", *(intArr + i));
    }
    printf("\n");
}


/****************************************************************
 * printState
 * Description: Prints gamestate info. The mode parameter takes
 * in an integer. If the integer is 0, the following variables
 * are printed at their current size
 * hand size
 * deck size
 * For any value other than zero, these are set to the max size. 
 ****************************************************************/
void printState(struct gameState * state, int maxMode)
{
    int i;
    int numPlayers;
    int handSize;
    int deckSize;
    int playedCards;

    if (maxMode == 0)
    {
        numPlayers = state->numPlayers;
        playedCards = state->playedCardCount;
    }
    else
    {
        numPlayers = MAX_PLAYERS;
        playedCards = MAX_DECK;
    }

    printf("\n");
    printf("Number of Players: %i\n", state->numPlayers);

    printf("Supply Count (%i 'treasure_map + 1' elements): \n\t", treasure_map + 1);
    intArrPrint(state->supplyCount, treasure_map + 1);

    printf("Embargo Tokens (%i 'treasure_map + 1' elements): \n\t", treasure_map + 1);
    intArrPrint(state->embargoTokens, treasure_map + 1);

    printf("Outposts Played: %i\n", state->outpostPlayed);
    printf("Outpost Turn: %i\n", state->outpostTurn);
    printf("Whose Turn: %i\n", state->whoseTurn);
    printf("Phase: %i\n", state->phase);
    printf("Number of Actions: %i\n", state->numActions);
    printf("Coins: %i\n", state->coins);
    printf("Number of Buys: %i\n", state->numBuys);

    printf("Hand (%i players): \n", numPlayers);
    for (i = 0; i < numPlayers; i++)
    {
        if (maxMode == 0)
            handSize = (state->handCount)[i];
        else
            handSize = MAX_HAND;
        printf("\t Player %i (handSize %i): ", i, handSize);
        intArrPrint((state->hand)[i], handSize);
    }

    printf("Hand Count (%i players elements): \n\t", numPlayers);
    intArrPrint(state->handCount, numPlayers);

    printf("Deck (%i players): \n", numPlayers);
    for (i = 0; i < numPlayers; i++)
    { 
        if (maxMode == 0)
            deckSize = (state->deckCount)[i];
        else
            deckSize = MAX_DECK;
        printf("\t Player %i (deckSize %i): ", i ,deckSize);       
        intArrPrint((state->deck)[i], deckSize);
    }

    printf("Deck Count (%i players): \n\t", numPlayers);
    intArrPrint(state->deckCount, numPlayers);

    printf("Discard (%i players): \n", numPlayers);
    for (i = 0; i < numPlayers; i++)
    {
        if (maxMode == 0)
            deckSize = (state->deckCount)[i];
        else
            deckSize = MAX_DECK;
        printf("\t Player %i (deckSize %i): ", i ,deckSize);       
        intArrPrint((state->discard)[i], deckSize);
    }

    printf("Discard Count (%i players): \n\t", numPlayers);
    intArrPrint(state->discardCount, numPlayers);

    printf("Played Cards (%i played cards): \n\t", playedCards);
    intArrPrint(state->playedCards, playedCards);

    printf("Played Card Count: %i\n", state->playedCardCount);
    printf("\n");
}

