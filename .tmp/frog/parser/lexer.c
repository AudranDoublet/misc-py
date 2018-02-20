#include <stdlib.h>
#include <ctype.h>
#include "../frog.h"

static tokeninfo token_err = {TOKEN_ERROR, NULL};
static tokeninfo token_eof = {TOKEN_EOF, NULL};

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

	node->tokentype = tokentype;
	return node;
}

struct automatanode *frog_automata(void)
{
	static struct automatanode *node = NULL;

	if(node)
		return node;

	node = create_automata(TOKEN_ERROR);
	simple_automata(node, "True", TOKEN_TRUE);
	simple_automata(node, "False", TOKEN_FALSE);
	simple_automata(node, "None", TOKEN_NULL);

	simple_automata(node, "if", TOKEN_IF);
	simple_automata(node, "else", TOKEN_ELSE);	
	simple_automata(node, "while", TOKEN_WHILE);
	simple_automata(node, "for", TOKEN_FOR);
	simple_automata(node, "foreach", TOKEN_FOREACH);
	simple_automata(node, "do", TOKEN_DO);
	simple_automata(node, "continue", TOKEN_CONTINUE);
	simple_automata(node, "break", TOKEN_BREAK);
	simple_automata(node, "try", TOKEN_TRY);
	simple_automata(node, "catch", TOKEN_CATCH);

	simple_automata(node, "return", TOKEN_RETURN);
	simple_automata(node, "yield", TOKEN_YIELD);
	simple_automata(node, "function", TOKEN_FUNCTION);
	simple_automata(node, "static", TOKEN_STATIC);
	simple_automata(node, "abstract", TOKEN_ABSTRACT);
	simple_automata(node, "private", TOKEN_PRIVATE);
	simple_automata(node, "protected", TOKEN_PROTECTED);

	simple_automata(node, "class", TOKEN_CLASS);

	simple_automata(node, "include", TOKEN_INCLUDE);
	simple_automata(node, "{", TOKEN_OPEN);
	simple_automata(node, "}", TOKEN_CLOSE);
	simple_automata(node, "(", TOKEN_SOPEN);
	simple_automata(node, ")", TOKEN_SCLOSE);
	simple_automata(node, ";", TOKEN_IEND);

	simple_automata(node, "==", TOKEN_CMPLE);
	simple_automata(node, "!=", TOKEN_CMPLE);
	simple_automata(node, "<", TOKEN_CMPLO);
	simple_automata(node, ">", TOKEN_CMPGT);
	simple_automata(node, "<=", TOKEN_CMPLE);
	simple_automata(node, ">=", TOKEN_CMPGE);

	simple_automata(node, "+", TOKEN_OPTPLS);

	return node;
}

tokenizer *create_tokenizer(nextline linegetter, void *file)
{
	tokenizer *tkz = malloc(sizeof(tokenizer));

	if(!tkz)
		return NULL;

	tkz->line = NULL;
	tkz->linepos = NULL;
	tkz->file = file;
	tkz->linegetter = linegetter;
	tkz->head = frog_automata();
	tkz->current = NULL;
	tkz->next = NULL;

	return tkz;
}

static int next_line(tokenizer *tkz)
{
	//free(tkz->line);
	tkz->line = (*tkz->linegetter)(tkz);
	tkz->linepos = tkz->line;

	return tkz->line != NULL;
}

static tokeninfo *ctoken(int type, FrogObject *value)
{
	tokeninfo *res = malloc(sizeof(tokeninfo));

	if(res == NULL)
	{
		return &token_err;
	}
	
	res->type = type;
	res->value = value;
	
	return res;
}

static inline fchar next_char(tokenizer *tkz)
{
	if(tkz->linepos && !*tkz->linepos)
	{
		tkz->linepos = NULL;
		return '\n';
	}


	if((!tkz->linepos || *tkz->linepos == 0) && !next_line(tkz))
		return 0;

	return *tkz->linepos;
}

static inline long digit16(fchar c)
{
	if(isdigit(c))
		return c - '0';
	else if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return -1;
}

static inline long digit10(fchar c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	return -1;
}

static inline long digit2(fchar c)
{
	if(c == '0')
		return 0;
	else if(c == '1')
		return 1;
	else return -1;
}

tokeninfo *read_int16(tokenizer *tkz)
{
	long digit = digit16(next_char(tkz));
	long result = 0;

	for(; digit != -1; digit = digit16(next_char(tkz)))
	{
		result = (result << 4) | digit;
		tkz->linepos++;
	}

	return ctoken(TOKEN_INTEGER, FromNativeInteger(result));
}

tokeninfo *read_int2(tokenizer *tkz)
{
	long digit = digit2(next_char(tkz));
	long result = 0;

	for(; digit != -1; digit = digit2(next_char(tkz)))
	{
		result = (result << 1) | digit;
		tkz->linepos++;
	}

	return ctoken(TOKEN_INTEGER, FromNativeInteger(result));
}

long read_int10s(tokenizer *tkz)
{
	long digit = digit10(next_char(tkz));
	long result = 0;

	for(; digit != -1; digit = digit10(next_char(tkz)))
	{
		result = (result * 10) + digit;
		tkz->linepos++;
	}

	return result;
}

tokeninfo *read_int10(tokenizer *tkz)
{
	long base = read_int10s(tkz);
	return ctoken(TOKEN_INTEGER, FromNativeInteger(base));
}

tokeninfo *read_number(tokenizer *tkz)
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

tokeninfo *read_string(tokenizer *tkz, fchar end, int escape)
{
	
	fchar c = next_char(tkz);

	for( ; c != end; tkz->linepos++, c = next_char(tkz))
	{
		if(escape && c == '\\')
		{

		}
	}

	return NULL;//FIXME
}

tokeninfo *read_keyword(tokenizer *tkz)
{
	struct automatanode *head;
	int alphanum, tokentype;
	FrogObject *res;
	struct strbuilder *builder = NULL;
	fchar c;

	head = tkz->head;
	alphanum = 1;
	builder = create_strbuilder();
	res = NULL;
	c = next_char(tkz);

	if(!builder)
	{
		goto error;
	}

	for( ; ; tkz->linepos++, c = next_char(tkz))
	{
		if(c > 127 || !head->nexts[c])
		{
			break;
		}

		alphanum = alphanum && (isalnum(c) || c == '_');
		add_strbuilder(builder, c);
		head = head->nexts[c];
	}

	tokentype = head->tokentype;

	if(head->tokentype == TOKEN_ERROR)
	{
		if(!alphanum)
		{
			free_strbuilder(builder);
			goto error;
		}
		else
		{
			for( ; ; tkz->linepos++, c = next_char(tkz))
			{
				if(c > 127 || (!isalnum(c) && c != '_'))
				{
					break;
				}

				add_strbuilder(builder, c);
			}

			res = utf32tostr(tostr_strbuilder(builder));

			if(res == NULL)
				tokentype = TOKEN_ERROR;
		}
	}

	free_strbuilder(builder);
	return ctoken(tokentype, res);

error:
	free_strbuilder(builder);
	return &token_err;
}

tokeninfo *read_token(tokenizer *tkz)
{
	fchar c;

start:
	if(tkz->linepos == NULL && !next_line(tkz))
	{
		goto eof;
	}

	while( isspace(next_char(tkz)) )
		tkz->linepos++;

	c = next_char(tkz);

	if(c == 0)
	{
		goto eof;
	}
	else if(c > 127)
	{
		goto error;
	}

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
	else if(c == '"' || c == '\'')
	{
		return read_string(tkz, c, 1);
	}
	else // keyword or identifier
	{
		return read_keyword(tkz);
	}

error:
	return &token_err;

eof:
	return &token_eof;
}

tokeninfo *next_token(tokenizer *tkz)
{
	if(tkz->next != NULL)
	{
		tkz->current = tkz->next;
		return tkz->current;
	}

	return tkz->current = read_token(tkz);
}

tokeninfo *peek_token(tokenizer *tkz)
{
	if(tkz->next != NULL)
	{
		return tkz->next;
	}

	return tkz->next = read_token(tkz);
}

int eat_token(tokenizer *tkz, int type)
{
	return next_token(tkz)->type == type;
}
