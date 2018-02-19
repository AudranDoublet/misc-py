#include <stdlib.h>
#include <string.h>
#include "strutils.h"

#define SIZE_NODE_STRBUILDER 	64

/**
 * utility used for this implementation of strbuilder
 */
struct buildernode
{
	char string[SIZE_NODE_STRBUILDER];
	size_t length;

	struct buildernode *next;
};

/**
 * create a buildernode with the provided prev value
 */
static struct buildernode *create_node(struct buildernode *prev)
{
	struct buildernode *r = calloc(sizeof(struct buildernode), 1);
	if(prev)
		prev->next = r;

	return r;
}

/**
 * clear a buildernode
 */
static void clear_node(struct buildernode *node)
{
	memset(node, 0, sizeof(struct buildernode));
}

/**
 * free a buildernode
 */
static void free_node(struct buildernode *curr)
{
	struct buildernode *next = NULL;

	while(curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
}

struct strbuilder *create_strbuilder(void)
{
	struct strbuilder *r = malloc(sizeof(struct strbuilder));

	if(!r)
	{
		return NULL;
	}

	r->length = 0;
	r->last = create_node(NULL);
	r->first = r->last;
	r->reserve = NULL;

	return r;
}

void clear_strbuilder(struct strbuilder *builder)
{
	struct buildernode *last = builder->last;
	struct buildernode *first = builder->first;

	if(last != first)
	{
		if(builder->reserve)
			last->next = builder->reserve;
		builder->reserve = first->next;
	}

	builder->last = first;
	clear_node(first);
}

void add_strbuilder(struct strbuilder *builder, char c)
{
	struct buildernode *curr = builder->last;

	if(curr->length == SIZE_NODE_STRBUILDER)
	{
		curr = create_node(curr);
		builder->last = curr;
	}

	curr->string[curr->length++] = c;
	builder->length++;
}

void append_strbuilder(struct strbuilder *builder, char *str)
{
	for( ; *str ; str++ )
	{
		add_strbuilder(builder, *str);
	}
}

char *tostr_strbuilder(struct strbuilder *builder)
{
	// allocate the memory which will be used to store the
	// result: length + 1 (for the final character \0
	char *str = malloc(sizeof(char) * (builder->length + 1));
	char *cur = str;
	struct buildernode *curr = builder->first;

	for( ; curr ; cur += curr->length, curr = curr->next )
	{
		memcpy(cur, curr->string, curr->length);
	}

	str[builder->length] = '\0';
	return str;
}

void free_strbuilder(struct strbuilder *builder)
{
	free_node(builder->first);
	free_node(builder->reserve);

	free(builder);
}
