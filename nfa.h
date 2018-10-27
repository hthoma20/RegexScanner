#ifndef NFA_H
#define NFA_H

typedef struct transition{
	char symbol;
	struct stateList* states;
} transition;

typedef struct transNode{
	transition* transition;
	struct transNode* next;
} transNode;

typedef struct transList{
	transNode* head;
} transList;

typedef struct state{
	transList* transitions;
} state;

typedef struct stateNode{
	state* state;
	struct stateNode* next;
} stateNode;

typedef struct stateList{
	stateNode* head;
	int size;
} stateList;

typedef struct nfa{
	stateList* Q;
	state* q0;
	stateList* F;
} nfa;

/* push the given node onto the head of the given list
 */
void pushStateNode(stateList* list, stateNode* node);

/* push given state to the head of the list
 */
void pushState(stateList* list, state* state);

/* push the given node onto the head of the given list
 */
void pushTransNode(transList* list, transNode* node);

/* crate an initalized state list
 */
stateList* makeStateList();

/* create an initialized state
 */
state* makeState();

/* creates an initialized nfa
 */
nfa* makeNFA();

/* return the list of states that reading symbol from initstate
 * would bring the machine to
 */
stateList* readSymbol(state* initState, char symbol);

/* adds a transion to the given initState
 * which moves the machine to finalState is symbol is read
 */
void addTransition(state* initState, char symbol, state* finalState);

/* tells whether state in in list 
 */
int containsState(stateList* list, state* state);


/* tells whether the given state is an accept state of the given nfa
 */
int isAcceptState(nfa* nfa, state* state);

/* completely free all contents of a state list
 * and the list itself
 */
void freeStateList(stateList* state);

/* completely free all contents of a state node and the node itself
 */
void freeStateNode(stateNode* node);

/* free contents of state and state itself
 */
void freeState(state* state);

/* completely free all contents of a transition list
 */
void freeTransList(transList* list);

/* completly free contents of a transition node
 */
void freeTransNode(transNode* node);

/* completly free all contents of an nfa
 * as well as the machine itself
 */
void freeNFA(nfa* nfa);

#endif
