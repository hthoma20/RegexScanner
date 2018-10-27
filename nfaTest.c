#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

void testNFA();
void testConcat();

int main(){
	testNFA();
	testConcat();
	printf("All tests passed\n");
	return 0;
}

void testNFA(){
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
	assert(isAcceptState(nfa,q2));
	assert(containsState(readSymbol(q0,'a'), q1)); 
	assert(containsState(readSymbol(q1,'b'), q2)); 
	assert(containsState(readSymbol(q1,'b'), q3));
	assert(readSymbol(q3,'b')->size == 0);
	
	freeNFA(nfa);
}

void testConcat(){
	//make two nfa's
	nfa* m1= makeNFA();
	state* q0= makeState();
	pushState(m1->Q, q0);
	state* q1= makeState();
	pushState(m1->Q, q1);
	addTransition(q0,'a',q1);
	m1->q0= q0;
	pushState(m1->F, q1);

	nfa* m2= makeNFA();
	state* q2= makeState();
	pushState(m2->Q, q2);
	state* q3= makeState();
	pushState(m2->Q, q3);
	addTransition(q2,'b',q3);
	m2->q0= q2;
	pushState(m2->F, q3);	

	//concatenate them
	concatNFAs(m1, m2);

	//check we have the right machine
	assert(m1->Q->size == 4);
	assert(m1->q0 == q0);
	assert(containsState(readSymbol(q1,'\0'), q2));
	assert(containsState(readSymbol(q0,'a'), q1));
	assert(containsState(readSymbol(q2,'b'), q3));
	assert(m1->F->size == 1);
}

