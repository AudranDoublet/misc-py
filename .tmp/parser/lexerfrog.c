#include <stdlib.h>
#include <stdio.h>
#include "lexerfrog.h"

#define LINE_RETURN "\n\ra"
#define WHITESPACES " \t\n\v\f\r"
#define DIGITS_DEC "0123456789"
#define DIGITS_HEX "0123456789abcdefABCDEF"
#define DIGITS_BIN "01"

static void set_automata(struct automatanode *node,
		struct automatanode *to, char a, char v)
{
	int i = (int) a;

	if(i < 0)
		i = ~i + 128;

	node->nexts[i] = to;
	node->values[i] = v;
}

static void create_numbers(struct automatanode *node)
{
	struct automatanode *hex = simple_automata(node, "0x", TOKEN_ERROR, 0);
	struct automatanode *bin = simple_automata(node, "0b", TOKEN_ERROR, 0);

	struct automatalist *list = create_autlist(node);
	struct automatanode *number = create_automata(TOKEN_INTDEC);
	struct automatanode *floatn = create_automata(TOKEN_DECIMAL);
	struct automatanode *floate = create_automata(TOKEN_DECIMAL);
	struct automatanode *floate2 = create_automata(TOKEN_DECIMAL);

	list = complex_automata(list, number, DIGITS_DEC, AUTOMATA_KLEENE); // dec
	list = complex_automata(list, floatn, ".", AUTOMATA_OPTIONAL); // float
	complex_sautomata(floatn, floatn, DIGITS_DEC, 0);

	list = complex_automata(list, floate, "eE", 0); // float exp
	list = complex_automata(list, floate, DIGITS_DEC, 0);
	list = complex_automata(list, floate2, ".", 0);
	list = complex_automata(list, floate2, DIGITS_DEC, 0);

	free_autlist(list);

	complex_sautomata(hex, create_automata(TOKEN_INTHEX), // hex
			DIGITS_HEX, AUTOMATA_KLEENE);
	complex_sautomata(bin, create_automata(TOKEN_INTBIN), // bin
			DIGITS_BIN, AUTOMATA_KLEENE);
}

static void create_string(struct automatanode *node, char *c, int type)
{
	struct automatanode *str = create_automata(TOKEN_ERROR);
	complex_sautomata(node, str, c, AUTOMATA_IGNORE);

	complex_sautomata(str, str, c, AUTOMATA_ALL | AUTOMATA_IGNORE);
	complex_sautomata(str, create_automata(type), c, AUTOMATA_IGNORE);
	
	if(type == TOKEN_STRING)
	{
		struct automatanode *stresc = create_automata(TOKEN_ERROR);
		set_automata(str, stresc, '\\', 0);
		complex_sautomata(str, stresc, "\\", AUTOMATA_IGNORE);

		set_automata(stresc, str, 'n', '\n');
		set_automata(stresc, str, 't', '\t');
		set_automata(stresc, str, '"', '"');
		set_automata(stresc, str, '\'', '\'');
	}
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

	complex_sautomata(node, node, WHITESPACES, AUTOMATA_IGNORE); // ignore whitespaces
	create_numbers(node);
	
	create_string(node, "'", TOKEN_STRING);
	create_string(simple_automata(node, "@", TOKEN_ERROR, 1), "'", TOKEN_ASTRING);
	create_string(node, "\"", TOKEN_STRING);
	create_string(simple_automata(node, "@", TOKEN_ERROR, 1), "\"", TOKEN_ASTRING);

	// comments
	struct automatanode *comment = simple_automata(node, "#", TOKEN_ERROR, AUTOMATA_IGNORE);
	complex_sautomata(comment, comment, LINE_RETURN, AUTOMATA_ALL | AUTOMATA_IGNORE);
	complex_sautomata(comment, node, LINE_RETURN, AUTOMATA_IGNORE);

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
			int max = i;

			for(int j = i + 1; j < 256; j++)
			{
				if(test->nexts[j] == test->nexts[i])
					max++;
				else break;
			}

			print(test->nexts[i], constr);
			if(i == max)
				printf("%i -> %i[label=\"\\%c\"]\n", test->constr,
				test->nexts[i]->constr, (char) i);
			else
			{
				printf("%i -> %i[label=\"\\%c-\\%c\"]\n", test->constr,
				test->nexts[i]->constr, (char) i, (char) max);
				i = max;
			}
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
