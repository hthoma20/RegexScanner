#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"

nfa* makeNFA(){
	nfa* nfa = malloc(sizeof(nfa));
	nfa->Q = makeStateList();
	nfa->F = makeStateList();
	return nfa;
}

//returns the first found state with the given label
//or NULL if no state has the given label
state* getLabeledState(nfa* m, int label){
	stateNode* curr;
	
	for(curr= m->Q->head; curr != NULL; curr= curr->next){
		if(curr->state->label == label){
			return curr->state;
		}
	}
	
	return NULL;
}

//put unique labels on the given NFA
void labelNFA(nfa* m){
	int label= 0;
	stateNode* curr;
	
	for(curr= m->Q->head; curr != NULL; curr= curr->next){
		curr->state->label= label;
		label++;
	}
}

//print the given NFA
void printNFA(nfa* m){
	if(m->Q->head == NULL){
		printf("Empty machine");
		return;
	}
	
	//print states and transitions
	stateNode* init;
	
	for(init= m->Q->head; init != NULL; init= init->next){
		state* state= init->state;
		if(m->q0 == state){
			printf("^");
		}
		if(isAcceptState(m, state)){
			printf("*");
		}
		printf("%x\n", state->label);
		transNode* trans;
		for(trans= state->transitions->head; trans != NULL; trans= trans->next){
			transition* t= trans->transition;
			printf("(%x,%c) -> ", state->label, t->symbol);
			stateNode* final;
			for(final= t->states->head; final != NULL; final= final->next){
				printf("%x, ", final->state->label);
			}
			printf("\n");
		}
		printf("\n");
	}
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
		addTransition(curr->state, EPSILON, m2->q0);
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

/* unions m1 and m2
*/
nfa* unionNFAs(nfa* m1, nfa* m2){
	nfa* u= makeNFA();
	
	//push all states into u's statelist
	stateNode* curr;
	//first m1's
	for(curr= m1->Q->head; curr != NULL; curr= curr->next){
		pushState(u->Q, curr->state);
	}
	//and m2's
	for(curr= m2->Q->head; curr != NULL; curr= curr->next){
		pushState(u->Q, curr->state);
	}
	
	//now push all accept states
	//first m1's
	for(curr= m1->F->head; curr != NULL; curr= curr->next){
		pushState(u->F, curr->state);
	}
	//and m2's
	for(curr= m2->F->head; curr != NULL; curr= curr->next){
		pushState(u->F, curr->state);
	}
	
	//finally, make the new start state and add epsilon transitions
	state* q0= makeState();
	addTransition(q0, EPSILON, m1->q0);
	addTransition(q0, EPSILON, m2->q0);
	
	pushState(u->Q, q0);
	u->q0= q0;
	
	return u;
}

/* alters m by making it accept L(m)* (star operation)
*/
void starNFA(nfa* m){
	//first add epsilon transions from each accept state to the
	//original start state
	stateNode* curr;
	for(curr= m->F->head; curr != NULL; curr= curr->next){
		addTransition(curr->state, EPSILON, m->q0);
	}
	
	//next, create a new state which which accepts and epsilon transitions
	//into the original start state
	state* q0= makeState();
	addTransition(q0, EPSILON, m->q0);
	pushState(m->F, q0);
	
	//add the state and set this state to be the start state
	pushState(m->Q, q0);
	m->q0= q0;
}

/* converts the given regex to an nfa
*/
nfa* regexToNFA(char* regex){
	int len= strlen(regex);
	
	//base case: string is empty
	if(len == 0){
		return epsilonNFA();
	}
	//base case: a single character
	if(len == 1){
		return charNFA(regex[0]);
	}
	
	//look for two things:
	// 1) a the first "piece" of the regex, 
	//		i.e. the first thing grouped in parens
	// 2) the first top-level union
	int unmatched= 0;
	int i;
	int unitIndex= 0;
	int unionIndex= -1;
	for(i=0; i < len; i++){
		if(regex[i] == '('){
			unmatched++;
		}
		else if(regex[i] == ')'){
			unmatched--;
			//if we are closing the first open paren
			if(unmatched == 0 && unitIndex == 0){
				unitIndex= i;
			}
		}
		//if we've found a top-level union
		else if(regex[i] == '|' && unmatched == 0){
			unionIndex= i;
			break;
		}
	}
	
	//if we found a top-level union
	if(unionIndex != -1){
		//get the regexs on each side of the union
		char* firstRegex= malloc(sizeof(char)*unionIndex);
		char* secondRegex= malloc(sizeof(char)*(len-unionIndex-1));
		
		strncpy(firstRegex, regex, unionIndex);
		strncpy(secondRegex, regex+unionIndex+1, len-unionIndex-1);
		
		//create the corresponding nfas
		nfa* firstNFA= regexToNFA(firstRegex);
		nfa* secondNFA= regexToNFA(secondRegex);
		
		//union them
		nfa* m= unionNFAs(firstNFA, secondNFA);
		
		//free the excess
		free(firstRegex);
		free(secondRegex);
		freeNFANotStates(firstNFA);
		freeNFANotStates(secondNFA);
	
		//return the union
		return m;
	}
	
	//if there was no union to worry about,
	
	nfa* firstPart;
	//if the first piece is not in parens
	if(unitIndex == 0){
		firstPart= charNFA(regex[0]);
	}
	else{
		//create the machine for the regex inside the parens
		char* newRegex= malloc(sizeof(char)*(unitIndex-1));
		strncpy(newRegex, regex+1, unitIndex-1);
		firstPart= regexToNFA(newRegex);
		//free(newRegex);
	}

	//now, check if we should star it
	if(regex[unitIndex+1] == '*'){
		starNFA(firstPart);
		//unitIndex is location of last character in first part
		unitIndex++;
	}
	
	//make the nfa for the second part
	nfa* secondPart= regexToNFA(regex+unitIndex+1);
	//finally, concat the first part with the rest
	concatNFAs(firstPart, secondPart);
	
	//free up the excess memory
	stateNode* curr;
	for(curr= secondPart->Q->head; curr != NULL; curr= curr->next){
		free(curr);
	}
	free(secondPart->Q);
	free(secondPart);
	
	//return the machine
	return firstPart;
}

/* returns the nfa that accepts nothing
*/
nfa* emptyNFA(){
	nfa* m= makeNFA();
	state* q= makeState();
	
	pushState(m->Q, q);
	m->q0= q;
	
	return m;
}

/* returns the nfa that accepts epsilon
*/
nfa* epsilonNFA(){
	nfa* m= emptyNFA();
	pushState(m->F, m->q0);
	
	return m;
}

/* returns the NFA that accepts a single string:
 * the given character
*/
nfa* charNFA(char ch){
	nfa* m= makeNFA();
	state* q0= makeState();
	state* qf= makeState();
	
	addTransition(q0, ch, qf);
	
	pushState(m->Q, q0);
	pushState(m->Q, qf);
	pushState(m->F, qf);
	m->q0 = q0;
	
	return m;
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

	return NULL;
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
	stateNode* curr= node->transition->states->head;
	while(curr != NULL){
		stateNode* temp= curr;
		curr= curr->next;
		free(temp);
	}
	
	free(node->transition->states);
	free(node->transition);
	free(node);
}

void freeNFA(nfa* nfa){
	freeStateList(nfa->Q);
	
	stateNode* curr= nfa->F->head;
	while(curr != NULL){
		stateNode* temp= curr;
		curr= curr->next;
		free(temp);
	}
	free(nfa->F);
	
	free(nfa);
}

void freeNFANotStates(nfa* nfa){
	stateNode* curr= nfa->Q->head;
	//free state nodes in Q
	while(curr != NULL){
		stateNode* temp= curr;
		curr= curr->next;
		free(temp);
	}
	//free Q itself
	free(nfa->Q);
	
	//free state nodes in F
	curr= nfa->F->head;
	while(curr != NULL){
		stateNode* temp= curr;
		curr= curr->next;
		
		free(temp);
	}
	//free F itself
	free(nfa->F);
	
	//free the nfa itself
	free(nfa);
}
