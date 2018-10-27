main: nfa.o
	gcc nfa.o -o main
test: nfa.o nfaTest.o
	gcc nfa.o nfaTest.o -o test
nfa.o: nfa.c nfa.h
	gcc -c nfa.c
nfaTest.o: nfaTest.c nfa.h
	gcc -c nfaTest.c

.PHONY: clean
clean:
	rm *.o
