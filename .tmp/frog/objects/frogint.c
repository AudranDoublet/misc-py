#include "../frog.h"

FrogType int_type;

FrogInt *FrogNativeInteger(long i)
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

	return result;
}

FrogObject *int_to_str(FrogObject *obj)
{
	//FrogInt *asint = obj;
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
		FrogErr_Compare(a, b);
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
	if(obtype(b)->toint)
	{
		b = (*ObType(b)->toint)(b);
	}
	else
	{
		FrogErr_Operator(a, b, "*");
		return null;
	}

	//fixme 

	return FromNativeInteger(FIValue(a) * FIValue(b));
}

FrogObject *int_div(FrogObject *a, FrogObject *b)
{
	return a; //FIXME return a floating number :D
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
		FrogErr_Operator(a, b, "**");
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

static inline long abs(long v)
{
	return v < 0 ? -v : v;
}

FrogObject *int_neg(FrogObject *a)
{
	return FromNativeInteger(-abs(FIValue(a)));
}

frogobject *int_pos(frogobject *a)
{
	return fromnativeinteger(abs(fivalue(a)));
}

FrogObject *int_inv(FrogObject *a)
{
	return FromNativeInteger(~FIValue(a));
}

frogobject *int_lshift(frogobject *a, frogobject *b)
{
	if(obtype(b)->toint)
	{
		b = (*obtype(b)->toint)(b);
	}
	else
	{
		frogerr_operator(a, b, "<<");
		return null;
	}

	if(FIValue(b) < 0)
	{
		frogerr
	}

	return fromnativeinteger(fivalue(a) * fivalue(b));
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


};

FrogType int_type = {
	{
		NULL,		// FIXME type
		-1		// refcnt
	}, 
	"int",			// type name
	NULL,			// FIXME getter
	NULL,			// setter
	int_hash,		// hash
	NULL,			// size
	int_to_str,		// tostr
	int_to_int,		// toint
	int_compare,		// compare
	&int_as_number,		// as number
	NULL,			// as sequence
	NULL,			// call	
};
