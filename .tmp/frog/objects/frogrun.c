#include "../frog.h"

FrogObject *FrogCall_Add(FrogObject *a, FrogObject *b)
{
	FrogType *type = ObType(a);

	if(type->as_number && type->as_number->add)
	{
		
	}
}

/*
FrogObject *FrogCall_Sub(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Mul(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Div(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_DivF(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Mod(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Pow(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Neg(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Pos(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Abs(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Inv(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_LShift(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_RShift(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_And(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Or(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Xor(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IAdd(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_ISub(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IMul(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IDiv(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IDivF(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IMod(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IPow(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_INeg(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IPos(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IAbs(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IInv(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_ILShift(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IRShift(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IAnd(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IOr(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_IXor(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_Assign(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_SeqGet(FrogObject *a, FrogObject *b);

FrogObject *FrogCall_SeqSet(FrogObject *a, FrogObject *b, FrogObject *c);

FrogObject *FrogCall_Call(FrogObject *a, FrogObject *b, FrogObject *c);**/
