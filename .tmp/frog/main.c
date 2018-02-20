#include <stdio.h>
#include "frog.h"
#include <readline/readline.h>
#include <readline/history.h>

fchar *nextline1(tokenizer *tkz)
{
	UNUSED(tkz);

	char *line;
	line = readline(">>> ");

	FrogString *str = (FrogString *) utf8tostr(line);

	if(str == NULL)
	{
		return NULL; //FIXME
	}

	if(str->length == 0)
	{
		free(str->str);
		free(str);

		return NULL;
	}

	fchar *res = str->str;
	free(str);

	return res;
}

int main(void)
{
	tokenizer *tkz = create_tokenizer(nextline1, NULL);
	tkz->file = stdin;
	tokeninfo *tkn;

	while((tkn = next_token(tkz)) && tkn->type >= 1)
	{
		if(tkn->value)
		{
			printf("%i (%li)\n", tkn->type, ((FrogInt *) tkn->value)->value);
		}
		else
			printf("%i\n", tkn->type);
	}

	return 0;
}
