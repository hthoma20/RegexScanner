main: scanre.o
	gcc -g scanre.o -o main
test: scanre.o scanreTest.o
	gcc -g nfa.o scanre.o scanreTest.o -o test
scanre.o: nfa.o scanre.c scanre.h
	gcc -g -c scanre.c
scanreTest.o: scanreTest.c scanre.h
	gcc -g -c scanreTest.c
nfa.o: nfa.c nfa.h
	gcc -g -c nfa.c
nfaTest.o: nfaTest.c nfa.h
	gcc -g -c nfaTest.c

.PHONY: clean
clean:
	rm *.o
