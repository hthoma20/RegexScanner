#ifndef NFA_H
#define NFA_H

//epsilon transitions
#define EPSILON '\0'

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
	int label;
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

/* returns the first state labeled with the given label
 * in the given nfa
 */
state* getLabeledState(nfa* m, int label);

/* puts unique labels on the states of the given NFA
*/
void labelNFA(nfa* m);

/* print the given nfa
*/
void printNFA(nfa* m);

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

/* converts the given regular expression to an NFA
	using the transformation
*/
nfa* regexToNFA(char* regex);

/* returns the nfa which accpets nothing
*/
nfa* emptyNFA();

/* returns the NFA which accpets {EPSILON}
*/
nfa* epsilonNFA();

/* returns the NFA which accepts the string
 * given by the single character, ch
 */
nfa* charNFA(char ch);

/* creates an initialized nfa
 */
nfa* makeNFA();

/* alters m1 to be the concatenation of m1 and m2
 * does not alter m2, however, the accept state set, F, of
 * m1 is exactly the set from m2, so messing with m2's
 * accept state may give unexpected behavior
 */
void concatNFAs(nfa* m1, nfa* m2);

/* creates the union of two nfa's and returns a pointer to the new machine
 * note that the union is created by makeing direct pointers to the states of
 * m1 and m2.
 * Therefore, altering the statelists may result in unexpected behavior
 */
nfa* unionNFAs(nfa* m1, nfa* m2);

/* alters m to be the machine which accpets
 * the language L(m)*
 */
void starNFA(nfa* m);

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

/* frees the structure memory for the machine nfa,
 * but does not free the memory for the states themselves,
 * nor their transitions
 */
void freeNFANotStates(nfa* nfa);

#endif
