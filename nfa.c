#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

int main(int argc, char* argv[]){
	stateList* list= makeStateList();

	freeStateList(list);
	return 0;
}

state* makeState(){
	state* state= malloc(sizeof(state));
	
	transList* transitions= malloc(sizeof(transList));
	transitions->head= NULL;

	state->transitions= transitions;

	return state;
}

stateList* readSymbol(state* initState, char symbol){
	stateList* finalStates;
	transNode* curr= initState->transitions->head;
	while(curr != NULL){
		if(curr->transition->symbol == symbol){
			return curr->transition->states;
		}
		curr= curr->next;
	}

	return makeStateList();
}

void addTransition(state* initState, char symbol, state* finalState){
	stateNode* finalStateNode= malloc(sizeof(stateNode));
	finalStateNode->state= finalState;
	
	//first check if symbol is already in the transition map
	transNode* curr= initState->transitions->head;
	while(curr != NULL){
		//if we found the symbol
		if(curr->transition->symbol == symbol){
			//add this state to the transition
			pushStateNode(curr->transition->states, finalStateNode);
			return;
		}
		curr= curr->next;
	}
	//here, we didn't find the symbol, so we add it
	transition* trans= malloc(sizeof(transition));
	trans->symbol= symbol;
	trans->states= malloc(sizeof(stateList));
	pushStateNode(trans->states, finalStateNode);

	transNode* node= malloc(sizeof(transNode));
	node->transition= trans;

	pushTransNode(initState->transitions, node);
}

void pushTransNode(transList* list, transNode* node){
	node->next= list->head;
	list->head= node;
}

/*
 * adds the given stateNode to the head
 * of the given stateList
 */
void pushStateNode(stateList* list, stateNode* node){
	node->next= list->head;
	list->head= node;
	list->size++;
}

stateList* makeStateList(){
	stateList* list= malloc(sizeof(stateList));
	list->head= NULL;
	list->size= 0;
	return list;
}

int isAcceptState(nfa* nfa, state* state){
	//traverse accept set, and look for the given state 
	stateNode* curr= nfa->F->head;
	while(curr != NULL){
		if(curr->state == state){
			return 1;
		}
		curr= curr->next;
	}

	return 0;
}

/* entirely frees all contents of the given stateList
 */
void freeStateList(stateList* list){
	stateNode* curr= list->head;
	stateNode* temp;
	while(curr != NULL){
		temp= curr;
		curr= curr->next;
		freeStateNode(temp);
	}

	free(list);
}

/* entirely frees all contents of the given stateNode
 */
void freeStateNode(stateNode* node){
	freeState(node->state);
	node->state= NULL;
	free(node);
}

/*
 * free the contents of the given state and the state itself
 */
void freeState(state* state){
	freeTransList(state->transitions);
	free(state);
}

void freeTransList(transList* list){
	transNode* curr= list->head;
	transNode* temp;
	while(curr != NULL){
		temp= curr;
		curr= curr->next;
		freeTransNode(temp);
	}

	free(list);
}

void freeTransNode(transNode* node){
	free(node->transition->states);
	free(node->transition);
	free(node);
}

void freeNFA(nfa* nfa){
	freeStateList(nfa->Q);
	free(nfa);
}
