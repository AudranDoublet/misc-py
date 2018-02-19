#include <stdlib.h>
#include <stdio.h>
#include "lexerfrog.h"

#define WHITESPACES " \t\n\v\f\r"
#define DIGITS_DEC "0123456789"
#define DIGITS_HEX "0123456789abcdefABCDEF"

static void comp_automata(struct automatanode *node, char *val,
		int tokentype, int addcars)
{
	struct automatalist *list = create_autlist(node);
	list = complex_automata(list, val, tokentype, addcars);
	free_autlist(node);
}

struct automatanode *create_frogautomata()
{
	struct automatanode *node = create_automata(TOKEN_ERROR);

	if(!node) return NULL;
	// integer, decimal, string
	simple_automata(node, "True", TOKEN_TRUE, 1);
	simple_automata(node, "False", TOKEN_FALSE, 1);
	simple_automata(node, "None", TOKEN_NULL, 1);

	simple_automata(node, "if", TOKEN_IF, 1);
	simple_automata(node, "else", TOKEN_ELSE, 1);	
	simple_automata(node, "while", TOKEN_WHILE, 1);
	simple_automata(node, "for", TOKEN_FOR, 1);
	simple_automata(node, "foreach", TOKEN_FOREACH, 1);
	simple_automata(node, "do", TOKEN_DO, 1);
	simple_automata(node, "continue", TOKEN_CONTINUE, 1);
	simple_automata(node, "break", TOKEN_BREAK, 1);
	simple_automata(node, "try", TOKEN_TRY, 1);
	simple_automata(node, "catch", TOKEN_CATCH, 1);

	simple_automata(node, "return", TOKEN_RETURN, 1);
	simple_automata(node, "yield", TOKEN_YIELD, 1);
	simple_automata(node, "function", TOKEN_FUNCTION, 1);
	simple_automata(node, "static", TOKEN_STATIC, 1);
	simple_automata(node, "abstract", TOKEN_ABSTRACT, 1);
	simple_automata(node, "private", TOKEN_PRIVATE, 1);
	simple_automata(node, "protected", TOKEN_PROTECTED, 1);

	simple_automata(node, "class", TOKEN_CLASS, 1);

	simple_automata(node, "include", TOKEN_INCLUDE, 1);
	simple_automata(node, "{", TOKEN_OPEN, 1);
	simple_automata(node, "}", TOKEN_CLOSE, 1);
	simple_automata(node, "(", TOKEN_SOPEN, 1);
	simple_automata(node, ")", TOKEN_SCLOSE, 1);
	simple_automata(node, ";", TOKEN_IEND, 1);

	simple_automata(node, "<=", TOKEN_CMPLE, 1);
	simple_automata(node, "<", TOKEN_CMPLO, 1);

	struct automatanode *hex = simple_automata(node, "0x", TOKEN_ERROR, 1);

	struct automatalist *list = create_autlist(node);
	struct automatanode *number = create_automata(TOKEN_INTEGER);
	struct automatanode *floatn = create_automata(TOKEN_DECIMAL);
	struct automatanode *floate = create_automata(TOKEN_DECIMAL);
	struct automatanode *floate2 = create_automata(TOKEN_DECIMAL);

	list = complex_automata(list, number, DIGITS_DEC, AUTOMATA_KLEENE);
	//list = complex_automata(list, number, DIGITS_DEC, 0);
	printf("tm");
	list = complex_automata(list, floatn, ".", AUTOMATA_OPTIONAL);
	comp_automata(floatn, floatn, DIGITS_DEC, 0);

	list = complex_automata(list, floate, "eE", 0);
	list = complex_automata(list, floate, DIGITS_DEC, 0);
	list = complex_automata(list, floate2, ".", 0);
	list = complex_automata(list, floate2, DIGITS_DEC, 0);

	free_autlist(list);
	list = create_list(hex);
	list = complex_automata(list, hex, DIGITS_DEC, 0);

	free_autlist(list);

	return node;
}

void print(struct automatanode *test, int *constr)
{
	if(test->constr != 0)
		return;
	
	test->constr = ++(*constr);

	for(int i = 0; i < 256; i++)
		if(test->nexts[i] != NULL)
		{
			print(test->nexts[i], constr);
			printf("%i -> %i[label=\"%c\"]\n", test->constr, test->nexts[i]->constr, (char) i);
		}
}

int main(void)
{
	int a = 0;
	printf("digraph{\n");
	struct automatanode *n = create_frogautomata();
	print(n, &a);
	printf("}");
}
