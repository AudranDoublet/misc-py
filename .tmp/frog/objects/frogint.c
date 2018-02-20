#include "../frog.h"

FrogType int_type;

FrogObject *FromNativeInteger(long i)
{
	FrogInt *result = malloc(sizeof(FrogInt));

	if(!result)
	{
		FrogErr_Memory();
		return NULL;
	}

	result->value = i;
	result->ob_base.type = &int_type;
	result->ob_base.refcnt = 0;

	return (FrogObject *) result;
}

FrogObject *int_to_str(FrogObject *obj)
{
	struct strbuilder *builder = create_strbuilder();
	long value = FIValue(obj);

	if(!builder || !addint_strbuilder(builder, value, 10))
		goto error;

	fchar *res = tostr_strbuilder(builder);

	if(!res)
		goto error;

	free_strbuilder(builder);
	return utf32tostr(res);
error:
	FrogErr_Memory();
	free_strbuilder(builder);
	return NULL;
}

FrogObject *int_to_int(FrogObject *obj)
{
	return obj;
}

long int_hash(FrogObject *obj)
{
	return FIValue(obj);
}

int int_compare(FrogObject *a, FrogObject *b)
{
	long vala = FIValue(a);

	if( !ObType(b)->toint)
	{
		FrogErr_Operator(a, b, "Comparaison");
		return -2;
	}
	else
	{
		b = (*ObType(b)->toint)(b);
	}

	long valb = FIValue(b);
	return (vala < valb) ? -1 : (vala > valb);
}

FrogObject *int_add(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "+");
		return NULL;
	}

	return FromNativeInteger(FIValue(a) + FIValue(b));
}

FrogObject *int_sub(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "-");
		return NULL;
	}

	return FromNativeInteger(FIValue(a) - FIValue(b));
}

FrogObject *int_mul(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "*");
		return NULL;
	}

	//fixme 

	return FromNativeInteger(FIValue(a) * FIValue(b));
}

FrogObject *int_div(FrogObject *a, FrogObject *b)
{
	return b ? b : a; //FIXME return a floating number :D
}

FrogObject *int_divfloor(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "/");
		return NULL;
	}

	if(FIValue(b) == 0)
	{
		FrogErr_Div0();
		return NULL;
	}

	return FromNativeInteger(FIValue(a) / FIValue(b));
}

FrogObject *int_mod(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "%");
		return NULL;
	}

	if(FIValue(b) == 0)
	{
		FrogErr_Div0();
		return NULL;
	}

	return FromNativeInteger(FIValue(a) % FIValue(b));
}

FrogObject *int_pow(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Op;
		if(exp % 2 == 1)
			res *= va;
		exp >>= 1;
	}

	return FromNativeInteger(res);
}

static inline long int_abs(long v)
{
	return v < 0 ? -v : v;
}

FrogObject *int_neg(FrogObject *a)
{
	return FromNativeInteger(-int_abs(FIValue(a)));
}

FrogObject *int_pos(FrogObject *a)
{
	return FromNativeInteger(int_abs(FIValue(a)));
}

FrogObject *int_inv(FrogObject *a)
{
	return FromNativeInteger(~FIValue(a));
}

FrogObject *int_lshift(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toinerator(a, b, "**");
		return NULL;
	}

	long exp = FIValue(b), va = FIValue(a);
	long res = exp == 0 ? 1 : va;

	while(exp > 1)
	{
		res *= res;
		if(exp % 2 == 1)
			res *= va;
		exp >>= 1;
	}

	return FromNativeInteger(res);
}

static inline long int_abs(long v)
{
	return v < 0 ? -v : v;
}

FrogObject *int_neg(FrogObject *a)
{
	return FromNativeInteger(-int_abs(FIValue(a)));
}

FrogObject *int_pos(FrogObject *a)
{
	return FromNativeInteger(int_abs(FIValue(a)));
}

FrogObject *int_inv(FrogObject *a)
{
	return FromNativeInteger(~FIValue(a));
}

FrogObject *int_lshift(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "<<");
		return NULL;
	}

	if(FIValue(b) < 0)
	{
		FrogErr_Value("negative shift count");
	}

	return FromNativeInteger(FIValue(a) << FIValue(b));
}

FrogObject *int_rshift(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, ">>");
		return NULL;
	}

	if(FIValue(b) < 0)
	{
		FrogErr_Value("negative shift count");
	}

	return FromNativeInteger(FIValue(a) >> FIValue(b));
}

FrogObject *int_and(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "&");
		return NULL;
	}

	return FromNativeInteger(FIValue(a) & FIValue(b));
}

FrogObject *int_or(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "|");
		return NULL;
	}

	return FromNativeInteger(FIValue(a) | FIValue(b));
}

FrogObject *int_xor(FrogObject *a, FrogObject *b)
{
	if(ObType(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "^");
		return NULL;
	}

	return FromNativeInteger(FIValue(a) ^ FIValue(b));
}

FrogAsNumber int_as_number = {
	int_add,
	int_sub,
	int_mul,
	int_div,
	int_divfloor,
	int_mod,
	int_pow,
	int_neg,
	int_pos,
	int_pos,
	int_inv,
	int_lshift,
	int_rshift,
	int_and,
	int_or,
	int_xor,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

FrogType int_type = {
	{
		-1,		// FIXME type
		NULL		// refcnt
	}, 
	"int",			// type name
	NULL,			// FIXME getter
	NULL,			// setter
	int_hash,		// hash
	NULL,			// size
	NULL,			// print
	int_to_str,		// tostr
	int_to_int,		// toint
	int_compare,		// compare
	&int_as_number,		// as number
	NULL,			// as sequence
	NULL,			// call	
};
