#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

void testNFA();
void testConcat();
void testUnion();
void testStar();
void testRegexToNFA();

int main(){
	testNFA();
	testConcat();
	testUnion();
	testStar();
	testRegexToNFA();
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
	assert(readSymbol(q3,'b') == NULL);
	
	freeNFA(nfa);
}
/*
void testNFA(){
	nfa* nfa= makeNFA();
	
	state* q0 = makeState();
	state* q1 = makeState();
	
	pushState(nfa->Q, q0);
	pushState(nfa->Q, q1);
	
	addTransition(q0,'a',q1);
	
	pushState(nfa->F, q1);
	
	nfa->q0= q0;
	
	freeNFA(nfa);
}*/

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
	assert(containsState(readSymbol(q1,EPSILON), q2));
	assert(containsState(readSymbol(q0,'a'), q1));
	assert(containsState(readSymbol(q2,'b'), q3));
	assert(m1->F->size == 1);
	
	freeNFA(m1);
}

void testUnion(){
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
	
	//union them
	nfa* u= unionNFAs(m1, m2);
	
	//make sure we have the right machine
	assert(u->Q->size == 5);
	
	stateList* q0Transitions= readSymbol(u->q0, EPSILON);
	assert(q0Transitions->size == 2);
	assert(containsState(q0Transitions, q0));
	assert(containsState(q0Transitions, q2));
	
	assert(u->F->size == 2);
	
	freeNFA(u);
}

void testStar(){
	//make an nfa
	nfa* m1= makeNFA();
	state* q0= makeState();
	pushState(m1->Q, q0);
	state* q1= makeState();
	pushState(m1->Q, q1);
	addTransition(q0,'a',q1);
	m1->q0= q0;
	pushState(m1->F, q1);
	
	//star it
	starNFA(m1);
	
	//make sure we have the right machine
	assert(m1->Q->size == 3);
	assert(m1->F->size == 2);
	assert(containsState(readSymbol(q1, EPSILON), q0));
	assert(containsState(readSymbol(m1->q0, EPSILON), q0));
}

void testRegexToNFANoUnion(){
	char* regex= "(ac)*b";
	
	nfa* m= regexToNFA(regex);
	
	labelNFA(m);
	//printNFA(m);
	
	assert(m->Q->size == 7);
	assert(m->F->size == 1);
	
	state* a= getLabeledState(m, 2);
	state* b= getLabeledState(m, 6);
	state* c= getLabeledState(m, 5);
	state* d= getLabeledState(m, 3);
	state* e= getLabeledState(m, 4);
	state* f= getLabeledState(m, 0);
	state* g= getLabeledState(m, 1);
	
	assert(m->q0 == a);
	assert(containsState(readSymbol(a, EPSILON), b));
	assert(containsState(readSymbol(a, EPSILON), f));
	
	assert(containsState(readSymbol(b, 'a'), c));

	assert(containsState(readSymbol(c, EPSILON), d));

	assert(containsState(readSymbol(d, 'c'), e));
	
	assert(containsState(readSymbol(e, EPSILON), f));

	assert(containsState(readSymbol(f, 'b'), g));
	
	assert(isAcceptState(m, g));
	
	freeNFA(m);
}

void testRegexToNFAUnion(){
	char* regex= "(a|b)*cd|a*b*";
	
	nfa* nfa= regexToNFA(regex);
	
	labelNFA(nfa);
	//printNFA(nfa);
	
	assert(nfa->Q->size == 18);
	assert(nfa->F->size == 2);
	
	state* a= getLabeledState(nfa, 0);
	state* b= getLabeledState(nfa, 13);
	state* c= getLabeledState(nfa, 12);
	state* d= getLabeledState(nfa, 11);
	state* e= getLabeledState(nfa, 10);
	state* f= getLabeledState(nfa, 9);
	state* g= getLabeledState(nfa, 8);
	state* h= getLabeledState(nfa, 17);
	state* i= getLabeledState(nfa, 16);
	state* j= getLabeledState(nfa, 14);
	state* k= getLabeledState(nfa, 15);
	state* l= getLabeledState(nfa, 3);
	state* m= getLabeledState(nfa, 1);
	state* n= getLabeledState(nfa, 2);
	state* o= getLabeledState(nfa, 5);
	state* p= getLabeledState(nfa, 7);
	state* q= getLabeledState(nfa, 6);
	state* r= getLabeledState(nfa, 4);
	
	assert(nfa->q0 == a);
	assert(containsState(readSymbol(a, EPSILON), l));
	assert(containsState(readSymbol(a, EPSILON), b));
	
	assert(containsState(readSymbol(b, EPSILON), h));
	assert(containsState(readSymbol(b, EPSILON), c));
	
	assert(containsState(readSymbol(c, EPSILON), f));
	assert(containsState(readSymbol(c, EPSILON), d));
	
	assert(containsState(readSymbol(d, 'b'), e));
	
	assert(containsState(readSymbol(e, EPSILON), c));
	assert(containsState(readSymbol(e, EPSILON), h));
	
	assert(containsState(readSymbol(f, 'a'), g));
	
	assert(containsState(readSymbol(g, EPSILON), c));
	assert(containsState(readSymbol(g, EPSILON), h));
	
	assert(containsState(readSymbol(h, 'c'), i));
	
	assert(containsState(readSymbol(i, EPSILON), j));
	
	assert(containsState(readSymbol(j, 'd'), k));
	
	assert(isAcceptState(nfa, k));
	
	assert(containsState(readSymbol(l, EPSILON), o));
	assert(containsState(readSymbol(l, EPSILON), m));
	
	assert(containsState(readSymbol(m, 'a'), n));
	
	assert(containsState(readSymbol(n, EPSILON), o));
	assert(containsState(readSymbol(n, EPSILON), m));
	
	assert(containsState(readSymbol(o, EPSILON), r));
	assert(containsState(readSymbol(o, EPSILON), p));
	
	assert(containsState(readSymbol(p, 'b'), q));
	
	assert(containsState(readSymbol(q, EPSILON), r));
	assert(containsState(readSymbol(q, EPSILON), p));
	
	assert(isAcceptState(nfa, r));
	
	free(nfa);
}

void testRegexToNFA(){
	// testRegexToNFAUnion();
	// testRegexToNFANoUnion();
}