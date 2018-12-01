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
	stateList* F;
	state* q0;
} nfa;

typedef struct configNode{
	state* state;
	int index;
	struct configNode* next;
} configNode;

typedef struct config{
	configNode* head;
} config;

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

/* concatenates m1 and m2
 * returns the concatenated machine and frees
 * excess memory.
 * alters m1 and m2 beyond use.
 */
nfa* concatNFAs(nfa* m1, nfa* m2);

/* creates the union of two nfa's and returns a pointer to the new machine
 * note that the union is created by makeing direct pointers to the states of
 * m1 and m2.
 * Also frees excess memory in m1 and m2, altering them beyond use
 */
nfa* unionNFAs(nfa* m1, nfa* m2);

/* alters m to be the machine which accpets
 * the language L(m)*
 */
nfa* starNFA(nfa* m);

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

/*creates a new make configNode and returns it
 */
configNode* makeConfigNode(state* state, int idx);

/* runs the string thru the nfa. 
 * returns null if nfa doesn't accept string
 * returns list of nodes it went thru to accept and index of string it was on during each node
 */
config* runNFA(nfa* m, state* currState, char* str, int index);


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

/* frees a config and all associated structure, but not the states
 * which the config points to
 */
void freeConfig(config* config);

#endif
