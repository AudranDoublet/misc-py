#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "../common/strutils.h"

struct automatanode *create_automata(int tokentype)
{
	struct automatanode *r = calloc(sizeof(struct automatanode), 1);
	r->tokentype = tokentype;

	return r;
}

struct automatanode *simple_automata(struct automatanode *node,
				char *val, int tokentype)
{
	for( ; *val ; val++ )
	{
		int i = (int) *val;
		
		if(node->nexts[i])
		{
			node = node->nexts[i];
		}
		else
		{
			node->nexts[i]  = create_automata(TOKEN_ERROR);
			node = node->nexts[i];
		}
	}

	if(tokentype != TOKEN_ERROR)
		node->tokentype = tokentype;

	return node;
}

static int next_line(tokenizer *tkz)
{
	free(tkz->line);
	tkz->linepos = parser->line = (*parser->linegetter)(parser);

	return tkz->line != NULL;
}

static inline char next_char(tokenizer *tkz)
{
	if(*tkz->linepos == 0 && !next_line())
		return 0;

	return *tkz->linepos;
}

static inline int digit16(char c)
{
	if(isdigit(c))
		return c - '0';
	else if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return -1;
}

struct tokeninfo *read_int16(tokenizer *tkz)
{
	int digit = digit16(next_char(tkz));
	int result = 0;

	for(; digit != -1; digit = digit16(next_char(tkz)))
	{
		result = (result << 4) | digit;
		tkz->linepos++;
	}

	return NULL; //FIXME
}

struct tokeninfo *read_int2(tokenizer *tkz)
{

}

struct tokeninfo *read_int10(tokenizer *tkz)
{
	//TODO if .eE => float
}

struct tokeninfo *read_number(tokenizer *tkz)
{
	if(next_char(tkz) == '0')
	{
		tkz->linepos++;

		if(next_char(tkz) == 'x')
		{
			tkz->linepos++;
			return read_int16(tkz);
		}
		else if(next_char(tkz) == 'b')
		{
			tkz->linepos++;
			return read_int2(tkz);
		}
	}

	return read_int10(tkz);
}

void read_escape_sequence()
{
	
}

struct tokeninfo *read_string(tokenizer *tkz, char end, int escape)
{
	
	char c = next_char(tkz);

	for( ; c != end; tkz->linepos++, c = next_char(tkz))
	{
		if(escape && c == '\\')
		{

		}
	}
}

struct tokeninfo *read_token(tokenizer *tkz)
{
	start:
	if(tkz->linepos == NULL && !next_line(tkz))
	{
		return EOF;//FIXME
	}

	while( isspace(next_char(tkz)) )
		tkz->linepos++;

	char c = next_char(tkz);

	if(c == 0)
		return EOF;//FIXME

	if(c == '#')
	{
		// one line comment, only skip line
		tkz->linepos = NULL;
		goto start; // recall function (but it's useless to enlarge stack)
	}
	else if(isdigit(c))
	{
		return read_number(tkz);
	}
	else if(isalpha(c) || c == '_')
	{
		// identifier or keyword
	}
	else if(c == '"' || c == '\'')
	{
		// string
	}
	else
	{
		// special character or error
	}

	return NULL;//FIXME
}

struct tokeninfo *next_token(tokenizer *tkz)
{
	if(tkz->next != NULL)
	{
		tkz->current = parser->next;
		return tkz->current;
	}

	return tkz->current = read_token(parser);
}

struct tokeninfo *peek_token(tokenizer *tkz)
{
	if(tkz->next != NULL)
	{
		return tkz->next;
	}

	return tkz->next = read_token(parser);
}

int eat_token(tokenizer *tkz, int type)
{
	return 0;
}

int main(void)
{
	return 0;
}
