CFLAGS= -Wall -fpic -coverage -lm -std=c99
DEBUG= -Wall -g -lm -std=c99 # For debugging

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)
testHelpers.o: testHelpers.h testHelpers.c dominion.o
	gcc -c testHelpers.c -g -Wall -fpic -lm -std=c99
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

unittests: unittest1.c unittest2.c unittest3.c unittest4.c dominion.o
	gcc -o unittest1 dominion.c rngs.c unittest1.c $(CFLAGS)
	gcc -o unittest2 dominion.c rngs.c unittest2.c $(CFLAGS)
	gcc -o unittest3 dominion.c rngs.c unittest3.c $(CFLAGS)
	gcc -o unittest4 dominion.c rngs.c unittest4.c $(CFLAGS)

cardtests: cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o
	gcc -o cardtest1 dominion.c rngs.c cardtest1.c $(CFLAGS)
	gcc -o cardtest2 dominion.c rngs.c cardtest2.c $(CFLAGS)
	gcc -o cardtest3 dominion.c rngs.c cardtest3.c $(CFLAGS)
	gcc -o cardtest4 dominion.c rngs.c cardtest4.c $(CFLAGS)

# Need .c for running debug not .c
randomtests: randomtestadventurer.c randomtestcard1.c randomtestcard2.c dominion.o rngs.o testHelpers.o
	echo "Results for running randomtestadventurer:" > randomtestresults.out
	gcc -o randomtestadventurer dominion.o rngs.o testHelpers.o randomtestadventurer.c $(CFLAGS)
	./randomtestadventurer >> randomtestresults.out
	gcov dominion.c >> randomtestresults.out
#	gcc -o randomtestadventurertest dominion.c rngs.c testHelpers.o randomtestadventurer.c $(DEBUG) 
	gcc -o randomtestcard1 dominion.o rngs.o testHelpers.o randomtestcard1.c $(CFLAGS)
	./randomtestcard1 >> randomtestresults.out
	gcov dominion.c >> randomtestresults.out
	gcc -o randomtestcard2 dominion.o rngs.o testHelpers.o randomtestcard2.c $(CFLAGS)
	./randomtestcard2 >> randomtestresults.out
	gcov dominion.c >> randomtestresults.out
#	gcc -o randomtestcard2 dominion.c rngs.c testHelpers.o randomtestcard2.c $(DEBUG) 

unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o rngs.o 
	
	echo "Results for running unittest:" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 dominion.o rngs.o unittest1.c $(CFLAGS)
	./unittest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	
	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 dominion.o rngs.o unittest2.c $(CFLAGS)
	./unittest2 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	
	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 dominion.o rngs.o unittest3.c $(CFLAGS)
	./unittest3 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	
	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 dominion.o rngs.o unittest4.c $(CFLAGS)
	./unittest4 >> unittestresults.out
	gcov dominion.c >> unittestresults.out

	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 dominion.o rngs.o cardtest1.c $(CFLAGS)
	./cardtest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out

	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 dominion.o rngs.o cardtest2.c $(CFLAGS)
	./cardtest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out

	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 dominion.o rngs.o cardtest3.c $(CFLAGS)
	./cardtest3 >> unittestresults.out
	gcov dominion.c >> unittestresults.out

	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 dominion.o rngs.o cardtest4.c $(CFLAGS)
	./cardtest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	
player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -rf *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest? unittestresults.out cardtest? randomtestadventurer randomtestadventurertest randomtestcard? *.dSYM
