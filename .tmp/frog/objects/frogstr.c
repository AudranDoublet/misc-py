#include "../frog.h"

#define UTF8_MASK1 0x1F
#define UTF8_MASK2 0x0F
#define UTF8_MASK3 0x07
#define UTF8_MASKN 0x4F

#define UTF8_NEXT(c1, c2, n)	\
	if(!(c1 = *(++utf8)))	\
		goto utf8error;	\
	else			\
		c2 |= (c1 & UTF8_MASKN) << n;

FrogObject *utf8tostr(char *str)
{
	struct strbuilder *builder = NULL;
	uint8_t *utf8 = NULL;
	FrogString *res = NULL;
	uint8_t c1 = 0;
	fchar c2 = 0;

	utf8 = (uint8_t *) str;
	builder = create_strbuilder();
	if(!builder) goto error;

	res = malloc(sizeof(FrogString));
	if(!res)
	{
		goto error;
	}

	res->length = 0;
	res->hash = 0;

	for(; *utf8; utf8++)
	{
		c1 = *utf8;

		if(c1 >> 7 == 0)
		{
			c2 = c1;
		}
		else if( ((c1 >> 5) & 1) == 0)
		{
			c2 = (c1 & UTF8_MASK1) << 6;
			UTF8_NEXT(c1, c2, 0);
		}
		else if( ((c1 >> 4) & 1) == 0)
		{
			c2 = (c1 & UTF8_MASK2) << 12;
			UTF8_NEXT(c1, c2, 6);
			UTF8_NEXT(c1, c2, 0);
		}
		else if( ((c1 >> 3) & 1) == 0)
		{
			c2 = (c1 & UTF8_MASK3) << 18;
			UTF8_NEXT(c1, c2, 12);
			UTF8_NEXT(c1, c2, 6);
			UTF8_NEXT(c1, c2, 0);
		}
		else
		{
			goto utf8error;
		}

		add_strbuilder(builder, c2);
	}

	res->str = tostr_strbuilder(builder);

	if(!res->str)
		goto error;

	res->length = builder->length;
	free_strbuilder(builder);

	return (FrogObject *) res;
utf8error:
	//FrogErr_Post("TypeError", "invalid utf8 string");
error:
	free_strbuilder(builder);
	free(res);
	return NULL;
}

FrogObject *utf32tostr(fchar *str)
{
	if(str == NULL)
		return NULL;

	FrogString *res = malloc(sizeof(FrogString));

	if(!str)
		return NULL;

	res->length = 0;
	res->str = str;
	res->hash = 0;

	for( ; *str ; str++ )
		res->length++;

	return (FrogObject *) res;
}
