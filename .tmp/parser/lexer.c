#include <stdlib.h>
#include "lexer.h"
#include "../common/strutils.h"

static char *get_all()
{
	static char v[256] = {0,};

	if(v[0] != 1)
	{
		for(int i = 0; i < 255; i++)
		{
			v[i] = (char) (i + 1);
		}

		v[255] = 0;
	}

	return v;
}

struct automatanode *create_automata(int tokentype)
{
	struct automatanode *r = calloc(sizeof(struct automatanode), 1);
	r->tokentype = tokentype;

	return r;
}

struct automatalist *create_autlist(struct automatanode *node)
{
	struct automatalist *r = malloc(sizeof(struct automatalist));

	r->next = NULL;
	r->value = node;

	return r;
}

static struct automatanode *automata_one(struct automatanode *node,
			struct automatanode *to, char c, int add)
{
	int i = (int) c;

	if(i < 0)
		i = ~i + 128;

	if(!node->nexts[i] && !to)
		node->nexts[i] = create_automata(TOKEN_ERROR);
	else if(!node->nexts[i])
		node->nexts[i] = to;

	if(add)
		node->values[i] = c;
	
	return node->nexts[i];
}

struct automatanode *simple_automata(struct automatanode *node,
				char *val, int tokentype,
				int addcars)
{
	for( ; *val ; val++ )
	{
		node = automata_one(node, NULL,  *val, addcars);
	}

	if(tokentype != TOKEN_ERROR)
		node->tokentype = tokentype;

	return node;
}

void complex_sautomata(struct automatanode *node,
				struct automatanode *target,
				char *val, int type)
{
	struct automatalist *l = create_autlist(node);
	l = complex_automata(l, target, val, type);
	free_autlist(l);
}

struct automatalist *complex_automata(struct automatalist *list,
				struct automatanode *target,
				char *val,
				int type)
{
	int optional = type & AUTOMATA_OPTIONAL, kleene = type & AUTOMATA_KLEENE;
	int all = type & AUTOMATA_ALL, ignore = type & AUTOMATA_IGNORE;
	char exclude[128] = {0,};

	if(all)
	{
		for(; *val; val++)
			exclude[(int)*val] = 1;
	
		val = get_all();
	}

	struct automatalist *res = optional ? list : NULL, *tmp = NULL;
	struct automatanode *node, *nodetmp;
	char *car;

	for( ; list ; list = list->next )
	{
		node = list->value;	
		node->constr = 0;

		if(tmp != NULL && !optional)
			free(tmp);

		for(car = val; *car; car++)
		{
			if(*car > 0 && exclude[(int) *car])
				continue;

			nodetmp = automata_one(node, target, *car, !ignore);

			if(nodetmp->constr == 0)
			{
				nodetmp->constr = 1;
				tmp = create_autlist(nodetmp);
				tmp->next = res;
				res = tmp;
			}
		}

		tmp = list;
	}

	if(kleene)
	{
		while(res->next)
			res = complex_automata(res, target, val, 0);
		
		complex_sautomata(target, target, val, 0);
	}

	return res;
}

void setresult_automata(struct automatalist *list, int tokentype)
{
	for( ; list ; list = list->next )
		list->value->tokentype = tokentype;
}

void free_autlist(struct automatalist *list)
{
	struct automatalist *next;

	while(list)
	{
		list->value->constr = 0;
		next = list->next;
		free(list);
		list = next;
	}
}
