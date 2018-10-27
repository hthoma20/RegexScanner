#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

int main(){
	nfa* nfa = makeNFA();	
	state* q0 = makeState();
	state* q1 = makeState();
	state* q2 = makeState();
	state* q3 = makeState();
	pushState(nfa->Q,q0);
	pushState(nfa->Q,q1);
	pushState(nfa->Q,q2);
	pushState(nfa->Q,q3);
	addTransition(q0,'a',q1);
	addTransition(q1,'b',q2);
	addTransition(q1,'b',q3);
	addTransition(q1,'c',q3);
	addTransition(q3,'a',q2);
	pushState(nfa->F,q2);
	nfa->q0 = q0;
	assert(nfa->Q->size == 4);
	assert(isAcceptState(nfa,q2);
	assert(containsState(readSymbol(q0,'a'), q1); 
	assert(containsState(readSymbol(q1,'b'), q2); 
	assert(containsState(readSymbol(q1,'b'), q3);
	assert((readSymbol(q3,'b')->size == 0); 
	return 0;
}
