#include <stdio.h>
#include <stdlib.h>
#include "nfa.h"

nfa* makeNFA(){
	nfa* nfa = malloc(sizeof(nfa));
	nfa->Q = makeStateList();
	nfa->F = makeStateList();
	return nfa;
}

/* concatenates m2 onto m1
 */
void concatNFAs(nfa* m1, nfa* m2){
	//first, add all states from m2
	stateNode* curr= m2->Q->head;
	while(curr!= NULL){
		pushState(m1->Q, curr->state);
		curr= curr->next;
	}

	//adding the states gives all the old transititions
	//now add the extra transitions for the concatenation
	for(curr= m1->F->head; curr!= NULL; curr= curr->next){
		addTransition(curr->state, '\0', m2->q0);
	}

	//remove accept states from m1
	//free list structure, but not the states themselves
	while(m1->F->head != NULL){
		stateNode* temp= m1->F->head;
		m1->F->head= m1->F->head->next;
		free(temp);
	}
	free(m1->F);

	//our accept states are exactly those from m2
	m1->F= m2->F;
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

void pushState(stateList* list, state* state){
	stateNode* stateNode = malloc(sizeof(stateNode));
	stateNode-> state = state;
	pushStateNode(list, stateNode);
}
stateList* makeStateList(){
	stateList* list= malloc(sizeof(stateList));
	list->head= NULL;
	list->size= 0;
	return list;
}

int containsState(stateList* list, state* state){
	//traverse accept set, and look for the given state 
	stateNode* curr= list->head;
	while(curr != NULL){
		if(curr->state == state){
			return 1;
		}
		curr= curr->next;
	}
	return 0;
}

int isAcceptState(nfa* nfa, state* state){
	return containsState(nfa->F, state);
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
