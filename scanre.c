#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "scanre.h"

//doubles the length of the given string by
//setting it to a new array, copying the original, and
//filling with \0
//returns the length of the longest string now possible
int doubleString(char** str, int srcSize){
	int newSize= 2*srcSize;
	char* dest= malloc(sizeof(char)*(newSize+1));
	
	strncpy(dest, *str, srcSize);
	memset(dest + srcSize, 0, newSize+1-srcSize); 
	
	free(*str);
	*str= dest;
	return newSize;
}

int fscanre(FILE* file, cRegex* cRegex, ...){
	if(!file) return NULL;
	
	//get the current position of the file in case we
	//fail and must reset it
	int beginFilePos= ftell(file);
	
	//count how many captures we are doing
	int numCaps= 0;
	captureNode* curr;
	for(curr= cRegex->captureHead; curr != NULL; curr= curr->next){
		numCaps++;
	}
	
	//make the array of capture variables, an array of char**
	char*** captureVars= malloc(sizeof(char**) * numCaps);
	
	//iterate the array and get the args out
	va_list argList;
	va_start(argList, cRegex);
	int i= 0;
	for(curr= cRegex->captureHead; curr != NULL; curr= curr->next){
		captureVars[i++]= va_arg(argList, char**);
	}
	va_end(argList);
	
	
	//make a string that will hold the current string to check
	int maxLen= 100;
	//set all to 0 so we know each string is null terminated
	char* currString= calloc(maxLen+1, sizeof(char));
	
	//start trying each string
	int len= 0;
	int ch= fgetc(file);
	
	while(ch != EOF){
		//before we store, make sure we have a place
		if(len == maxLen){
			maxLen= doubleString(&currString, maxLen);
		}
		
		currString[len]= ch;
		len++;
		
		//try to match and capture the current string
		if(scanString(cRegex, currString, captureVars)){
			free(captureVars);
			return 1;
		}
		
		//get the next character
		ch= fgetc(file);
	}
	
	//if we get out of the loop, it is a failure
	//reset the file pointer and return false
	fseek(file, 0, beginFilePos);
	free(captureVars);
	return 0;
}

int scanString(cRegex* cRegex, char* str, char*** captureVars){
	config* config = runNFA(cRegex->m, cRegex->m->q0, str, 0);
	//if the string doesn't accept in NFA return 0
	if(config == NULL){
		return 0;
	}
	
	char** saveVar = NULL;
	//outer loop goes through each capture node
	int i = 0;
	captureNode* captureNodeTemp = NULL;
	for(captureNodeTemp=cRegex->captureHead; captureNodeTemp != NULL; captureNodeTemp = captureNodeTemp->next){
		saveVar = captureVars[i];
		i++;
		int startIndex;
		int endIndex;
		configNode* configTemp = NULL;
		
		//goes through and finds the beginning of the capture in the string
		for( configTemp=config->head; configTemp != NULL; configTemp=configTemp->next){
			if(configTemp->state == captureNodeTemp->begin){
				startIndex = configTemp->index;
				break;
			}
		}
		//goes through config list and finds the last occurance of ends
		for( configTemp=config->head; configTemp != NULL; configTemp=configTemp->next){
			state* endsTemp = NULL;
			//check to see if the current configNode's state is equal to end temp
			if(containsState(captureNodeTemp->end, configTemp->state)){
				endIndex = configTemp->index;
			}
		}
		*saveVar = malloc(sizeof(char)*(endIndex-startIndex+1));
		strncpy(*saveVar,str+startIndex, endIndex-startIndex);
		(*saveVar)[endIndex-startIndex] = '\0';
	}
	return 1;
}
	
cRegex* makeCRegex(char* regex){
	//make the cRegex
	cRegex* cRegex= malloc(sizeof(struct cRegex));
	cRegex->captureHead= NULL;
	
	/*
		step 1:
			make regex for each thing in <thing>
			note the start state
			note the endstates
			
		step 2:
			concatenate all things from step 1
	*/
	
	//the entire NFA so far
	nfa* runningNFA= NULL;
	//the nfa we are currently builing
	nfa* currNFA= NULL;
	
	//walk the string, looking for things in <>
	int index;
	int end= strlen(regex)+1;
	//walk backward so that pushing captureNodes is in correct order
	for(index= end-1; index >= 0; index--){
		if(regex[index] == '<' || regex[index] == '>'){			
			//make the machine
			char* substring= malloc(sizeof(char) * (end-index));
			substring= strncpy(substring, regex+index+1, end-index-1);
			substring[end-index-1]= '\0';
			
			currNFA= regexToNFA(substring);
			
			free(substring);
			
			//if we want to capture this piece
			if(regex[index] == '<'){
				//make the capture node
				captureNode* node= malloc(sizeof(struct captureNode));
				node->end = makeStateList();
				node->begin= currNFA->q0;
				//copy the accept states of F into the endstates of node
				stateNode* currStateNode= currNFA->F->head;
				for(;currStateNode != NULL; currStateNode= currStateNode->next){
					pushState(node->end, currStateNode->state);
				}
				//adding capture node to the head of cRegex capture head
				node->next = cRegex->captureHead;
				cRegex->captureHead = node;
			}
			
			//concat this piece with the running nfa
			if(runningNFA == NULL){
				runningNFA= currNFA;
			}
			else{
				runningNFA= concatNFAs(currNFA, runningNFA);
			}
			
			end= index;
		}
	}
	if(end != 0){
		char* substring= malloc(sizeof(char) * (end-index));
		substring= strncpy(substring, regex+index+1, end-index-1);
		substring[end-index-1]= '\0';
			
		currNFA= regexToNFA(substring);
		
		free(substring);
		
		//concat this piece with the running nfa
		if(runningNFA == NULL){
			runningNFA= currNFA;
		}
		else{
			runningNFA= concatNFAs(currNFA, runningNFA);
		}
	}
	cRegex->m= runningNFA;
	return cRegex;
}

void freeCRegex(cRegex* cRegex){
	captureNode* temp = cRegex->captureHead;
	while(temp != NULL){
		temp=temp->next;
		freeCaptureNode(cRegex->captureHead);
		cRegex->captureHead = temp;
		
	}
	freeNFA(cRegex->m);
	free(cRegex);
}

void freeCaptureNode(captureNode* node){
	stateNode* curr= node->end->head;
	//free state nodes in end
	while(curr != NULL){
		stateNode* temp= curr;
		curr= curr->next;
		free(temp);
	}
	//free end itself
	free(node->end);
	
	free(node);
	
}