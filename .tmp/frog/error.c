#include <stdio.h>
#include "frog.h"

static char buffer[1024];
static FrogObject *error = NULL;

static FrogObject *create_error(char *name, char *message)
{
	FrogError *err = malloc(sizeof(FrogError));

	if(err == NULL)
	{
		// not enough memory to create an error...
		//TODO better handling ?
		printf("out of memory");
		exit(-1);
	}

	err->name = utf8tostr(name);
	err->message = utf8tostr(message);

	return (FrogObject *) err;
}

FrogObject *FrogErr_Get(void)
{
	return error;
}

void FrogErr_Post(char *name, char *message)
{
	error = create_error(name, message);
}

void FrogErr_Operator(FrogObject *a, FrogObject *b, char *op)
{
	sprintf(buffer, "%s not supported between '%s' and '%s'",op, 
	ObType(a)->name, ObType(b)->name);
	FrogErr_Post("TypeError", buffer);
}
