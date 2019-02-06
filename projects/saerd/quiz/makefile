CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc	testme.c -o testme $(CFLAGS)
	./testme
	
clean:
	rm -f *.gcov *.gcda *.gcno testme 
