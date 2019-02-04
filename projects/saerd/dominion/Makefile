CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)


unittestresults: dominion.c dominion.h rngs.o dominion_helpers.h unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c
	echo "Assignment 3 Test Results" >> unittestresults.out	
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Unit Test 1 (whoseTurn Function): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h unittest1.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Unit Test 2 (supplyCount Function): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h unittest2.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Unit Test 3 (fullDeckCount Function): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h unittest3.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Unit Test 4 (isGameOver Function): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h unittest4.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Card Test 1 (smithy): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h cardtest1.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Card Test 2 (adventurer): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h cardtest2.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Card Test 3 (sea_hag): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h cardtest3.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Card Test 4 (great_hall): " >> unittestresults.out
	gcc	dominion.c dominion.h rngs.o dominion_helpers.h cardtest4.c $(CFLAGS)
	./a.out >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out

all: playdom player unittestresults 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
