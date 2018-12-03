#ifndef SCANRE_H
#define SCANRE_H

#include "nfa.h"

typedef struct captureNode{
	state* begin;
	stateList* end;
	
	struct captureNode* next;
} captureNode;

//capturable regular expression
typedef struct cRegex{
	nfa* m;
	
	captureNode* captureHead;
	
} cRegex;

//returns a cRegex structure based on the given regular expression
cRegex* makeCRegex(char* regex);

//completely frees all memory accosiated with the given cRegex
void freeCRegex(cRegex* cRegex);

void freeCaptureNode(captureNode* node);

#endif