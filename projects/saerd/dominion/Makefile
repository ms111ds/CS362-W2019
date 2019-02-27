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

randomTest1: dominion.o rngs.o randomtestcard1.c
	gcc -o randomTest1 dominion.o rngs.o randomtestcard1.c -g $(CFLAGS)

randomTest2: dominion.o rngs.o randomtestcard2.c
	gcc -o randomTest2 dominion.o rngs.o randomtestcard2.c -g $(CFLAGS)

randomTest3: dominion.o rngs.o randomtestcard3.c
	gcc -o randomTest3 dominion.o rngs.o randomtestcard3.c -g $(CFLAGS)


randomTestsAll: randomTest1 randomTest2 randomTest3
	echo "---------- test results ----------" > randomtestcard1.out
	./randomTest1 10 >> randomtestcard1.out 2>&1
	echo >> randomtestcard1.out
	echo "---------- gcov results ----------" >> randomtestcard1.out
	gcov dominion.c -b >> randomtestcard1.out
	echo >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out
	
	echo "---------- test results ----------" > randomtestcard2.out
	./randomTest2 20 >> randomtestcard2.out 2>&1
	echo >> randomtestcard2.out
	echo "---------- gcov results ----------" >> randomtestcard2.out
	gcov dominion.c -b >> randomtestcard2.out
	echo >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out
	
	echo "---------- test results ----------" > randomtestcard3.out
	./randomTest3 5 >> randomtestcard3.out 2>&1
	echo >> randomtestcard3.out
	echo "---------- gcov results ----------" >> randomtestcard3.out
	gcov dominion.c -b >> randomtestcard3.out
	echo >> randomtestcard3.out
	cat dominion.c.gcov >> randomtestcard3.out

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
