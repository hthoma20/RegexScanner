#ifndef SCANRE_H
#define SCANRE_H

#include <stdio.h>
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

//takes the file to scan from, a cregex and 
//a list of char** which are pointers to unallocated strings
//fscanre will attempt to populate the char*s with the strings that
//were "captured" by the cRegex
//returns a boolean as to whether the scan was successful
int fscanre(FILE* file, cRegex* cRegex, ...);

int scanre(cRegex* cRegex, ...);

//takes a cRegex, a string, and an array of char**
//attempts to match the string with the cRegex
//if the string matches, it allocates memory for and populates each
//given char** with the captured string, and returns 1
//if the string does not match, returns 0 and does nothing with the char**s

int scanString(cRegex* cRegex, char* str,  char*** captureVars);

//returns a cRegex structure based on the given regular expression
cRegex* makeCRegex(char* regex);

//completely frees all memory accosiated with the given cRegex
void freeCRegex(cRegex* cRegex);

void freeCaptureNode(captureNode* node);

#endif