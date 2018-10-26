main: nfa.o
	gcc nfa.o -o main
nfa.o: nfa.c nfa.h
	gcc -c nfa.c

.PHONY: clean
clean:
	rm *.o
