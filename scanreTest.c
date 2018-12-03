#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "scanre.h"

void testMakeCRegex();

int main(){
	testMakeCRegex();
	printf("All tests passed\n");
}

void testMakeCRegex(){
	//make a cRegex
	cRegex* testRegex = makeCRegex("<a>b");
	labelNFA(testRegex->m);
	state* a = getLabeledState(testRegex->m, 3);
	state* b = getLabeledState(testRegex->m, 2);
	state* c = getLabeledState(testRegex->m, 0);
	state* d = getLabeledState(testRegex->m, 1);
	
	assert(testRegex->captureHead != NULL);
	assert(testRegex->captureHead->begin == a);
	assert(testRegex->captureHead->end->size == 1);
	assert(containsState(testRegex->captureHead->end, b));
	
	
	freeCRegex(testRegex);
	
	testRegex = makeCRegex("a<b|c*>d");
	labelNFA(testRegex->m);
	
	a = getLabeledState(testRegex->m, 0);
	b = getLabeledState(testRegex->m, 1);
	c = getLabeledState(testRegex->m, 2);
	state* k = getLabeledState(testRegex->m, 3);
	d = getLabeledState(testRegex->m, 4);
	state* e = getLabeledState(testRegex->m, 5);
	state* f = getLabeledState(testRegex->m, 6);
	state* g = getLabeledState(testRegex->m, 7);
	state* h = getLabeledState(testRegex->m, 8);
	state* i = getLabeledState(testRegex->m, 9);
	state* j = getLabeledState(testRegex->m, 10);
		
	assert(testRegex->captureHead != NULL);
	assert(testRegex->captureHead->begin == f);
	assert(testRegex->captureHead->end->size == 2);
	assert(containsState(testRegex->captureHead->end, a));
	assert(containsState(testRegex->captureHead->end, c));	
	
	freeCRegex(testRegex);
}