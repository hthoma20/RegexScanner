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
	testRunNFA();
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
	m1= concatNFAs(m1, m2);

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
	m1= starNFA(m1);
	
	//make sure we have the right machine
	assert(m1->Q->size == 3);
	assert(m1->F->size == 2);
	assert(containsState(readSymbol(q1, EPSILON), q0));
	assert(containsState(readSymbol(m1->q0, EPSILON), q0));
	
	freeNFA(m1);
}

void testRegexToNFANoUnion(){
	char* regex= "(ac)*b";
	
	nfa* m= regexToNFA(regex);
	
	labelNFA(m);
	
	assert(m->Q->size == 7);
	assert(m->F->size == 1);
	
	//to create this test, we printed the nfa with labels
	//and verified that it was the correct NFA
	//now we are asserting that we get that same NFA
	//(the labels should be the same)
	
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
	
	assert(nfa->Q->size == 18);
	assert(nfa->F->size == 2);
	
	//to create this test, we printed the nfa with labels
	//and verified that it was the correct NFA
	//now we are asserting that we get that same NFA
	//(the labels should be the same)
	
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
	
	freeNFA(nfa);
}

void testRegexToNFA(){
	testRegexToNFANoUnion();
	testRegexToNFAUnion();
}

void testRunNFADeterministic(){
	//create an nfa
	nfa* nfa= createNFA();
	
	state* a= makeState();
	state* b= makeState():
	state* c= makeState();
	
	addTransition(a, 'a', b);
	addTransition(b, 'b', c);
	addTransition(c, 'a', a);
	
	pushState(nfa->Q, a);
	pushState(nfa->Q, b);
	pushState(nfa->Q, c);
	
	pushState(nfa->F, b);
	
	nfa->q0= q0;
	
	//run the nfa on a given string
	config* config= runNFA(nfa, nfa->q0, "a", 0);
	
	//check that the config is correct
	configNode* curr= config->head;
	assert(curr->state == a);
	assert(curr->index == 0);
	curr= curr->next;
	assert(curr->state == b);
	assert(curr->index == 2);
	curr= curr->next;
	assert(curr == NULL);
	
	freeConfig(config);
	
	//test a harder string
	config= runNFA(nfa, nfa->q0, "abaa", 0);
	
	//check that the config is correct
	configNode* curr= config->head;
	assert(curr->state == a);
	assert(curr->index == 0);
	curr= curr->next;
	assert(curr->state == b);
	assert(curr->index == 1);
	curr= curr->next;
	assert(curr->state == c);
	assert(curr->index == 2);
	curr= curr->next;
	assert(curr->state == a);
	assert(curr->index == 3);
	curr= curr->next;
	assert(curr->state == b);
	assert(curr->index == 4);
	curr= curr->next;
	assert(curr == NULL);
	
	freeConfig(config);
	
	//test a fail
	config= runNFA(nfa, nfa->q0, "ab", 0);
	assert(config == NULL);
	
	freeNFA(nfa);
}

void testRunNFA(){
	testRunNFADeterministic();
}