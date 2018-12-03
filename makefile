main: nfa.o scanre.o demo.o
	gcc -g nfa.o scanre.o demo.o -o main
test: test.o nfaTest.c scanreTest.c scanre.o nfa.o
	gcc -g nfa.o scanre.o test.o -o test

demo.o: demo.c scanre.h
	gcc -g -c demo.c
scanre.o: scanre.c scanre.h nfa.h
	gcc -g -c scanre.c
nfa.o: nfa.c nfa.h
	gcc -g -c nfa.c
test.o: test.c nfaTest.h scanreTest.h
	gcc -g -c test.c
scanreTest.o: scanreTest.c scanreTest.h scanre.h
	gcc -g -c scanreTest.c
nfaTest.o: nfaTest.c nfaTest.h nfa.h
	gcc -g -c nfaTest.c

.PHONY: clean
clean:
	rm -f *.o
	rm -f main
	rm -f test
