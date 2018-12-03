#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "scanre.h"

void testMakeCRegex();
void testScanString();
void testFscanre();

int main(){
	testMakeCRegex();
	testScanString();
	testFscanre();
	printf("All tests passed\n");
	
	// cRegex* creg= makeCRegex("<a*>b");
	// nfa* m= regexToNFA("a*b");
	// config* config= runNFA(m, m->q0, "ab", 0);
	
	// freeNFA(m);
	// freeConfig(config);
	// freeCRegex(creg);
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

void testScanString(){
	//make the cRegex
	cRegex* creg= makeCRegex("<a*>b");
	
	char* var;
	
	char** list[]= {&var};
	
	int success= scanString(creg, "aab", list);
	
	// assert(success);
	// assert(strcmp(var, "aa") == 0);
	
	free(var);
	
	success= scanString(creg, "b", list);

	assert(success);
	assert(strcmp(var, "") == 0);
	
	free(var);
	freeCRegex(creg);
	//try another regex
	creg= makeCRegex("<a*>b<c*>");
	char* var2;
	char** list2[]= {&var, &var2};
	
	success= scanString(creg, "aabccc", list2);
	
	assert(success);
	assert(strcmp(var, "aa") == 0);
	assert(strcmp(var2, "ccc") == 0);
	
	free(var);
	free(var2);
	
	//try another string
	success= scanString(creg, "bccc", list2);
	assert(success);
	assert(strcmp(var, "") == 0);
	assert(strcmp(var2, "ccc") == 0);
	
	
	free(var);
	free(var2);
	
	//try another string
	success= scanString(creg, "ab", list2);
	
	assert(success);
	assert(strcmp(var, "a") == 0);
	assert(strcmp(var2, "") == 0);
	
	free(var);
	free(var2);
	
	//try another string
	success= scanString(creg, "b", list2);
	
	assert(success);
	assert(strcmp(var, "") == 0);
	assert(strcmp(var2, "") == 0);
	
	free(var);
	free(var2);
	
	freeCRegex(creg);
	
}

void testFscanre(){
	//get the file
	FILE* file= fopen("test1.txt", "r");
	
	//make the cRegex
	cRegex* creg= makeCRegex("<a*>b");
	
	char* var;
	
	//try a scan
	int success= fscanre(file, creg, &var);
	
	assert(success);
	assert(strcmp(var, "aaa") == 0);
	
	free(var);
	
	//try another
	success= fscanre(file, creg, &var);
	
	assert(success);
	assert(strcmp(var, "a") == 0);
	
	free(var);
	
	//try another
	success= fscanre(file, creg, &var);
	
	assert(success);
	assert(strcmp(var, "") == 0);
	
	free(var);
	
	freeCRegex(creg);
	
	fclose(file);
}