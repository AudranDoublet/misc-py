#ifndef __PARSER_H__
#define __PARSER_H__
#include "../common/message.h"

#define AUTOMATA_KLEENE		1
#define AUTOMATA_OPTIONAL	2
#define AUTOMATA_ALL		4
#define AUTOMATA_IGNORE		8

#define TOKEN_ERROR		0
#define TOKEN_EOF		1

typedef void (*addcharacter)(void *currentvalue, char car);

struct tokeninfo
{
	int type;
	void *value;
};

struct automatanode
{
	struct automatanode *nexts[256];
	char values[256];
	int tokentype;
	int constr;
};

struct automatalist
{
	struct automatalist *next;
	struct automatanode *value;
};

struct parserinfo
{
	struct automatanode *head;
	messagelistener listener;

	struct tokeninfo *current;
	struct tokeninfo *next;
};

/**
 =========
 Function for building automaton
 =========
**/

/**
 * Create an new automata node with given the token type
 */
struct automatanode *create_automata(int tokentype);

/**
 * Create an automata list from the given node
 */
struct automatalist *create_autlist(struct automatanode *node);

/**
 * Add a simple text sequence to an automata
 */
struct automatanode *simple_automata(struct automatanode *node,
				char *val, int tokentype,
				int addcars);

/**
 * Add a complex text sequence to an automata, using a single node
 */
void complex_sautomata(struct automatanode *node,
				struct automatanode *target,
				char *val,
				int type);

/**
 * Add a complex text sequence to an automata, using a node list
 */
struct automatalist *complex_automata(struct automatalist *list,
				struct automatanode *target,
				char *val,
				int type);

/**
 * Set the return type for all node of the list
 */
void setresult_automata(struct automatalist *list, int tokentype);

/**
 * Free the given automata list
 */
void free_autlist(struct automatalist *list);

/**
 * Free the given automata
 */
void free_automata(struct automatanode *automata);

/**
 =========
 Function for tokens
 =========
**/

/**
 * Change value of parserinfo->current (return the newvalue) and set parserinfo->next to NULL.
 */
struct tokeninfo *next_token(struct parserinfo *parser);

/**
 * Change value of parserinfo->next and return the new value
 */
struct tokeninfo *peek_token(struct parserinfo *parser);

/**
 * Verify that the result of next_token is of the right
 * token type.
 */
int eat_token(struct parserinfo *parser, int type);

/**
 * Free the given parser
 */
void free_parser(struct parserinfo *parser);
#endif
