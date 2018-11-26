main: nfa.o
	gcc -g nfa.o -o main
test: nfa.o nfaTest.o
	gcc -g nfa.o nfaTest.o -o test
nfa.o: nfa.c nfa.h
	gcc -g -c nfa.c
nfaTest.o: nfaTest.c nfa.h
	gcc -g -c nfaTest.c

.PHONY: clean
clean:
	rm *.o
