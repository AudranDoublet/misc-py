#ifndef __FROGERROR_H__
#define __FROGERROR_H__

typedef struct {
	FrogObjHead
	FrogObject *name;
	FrogObject *message;
} FrogError;

FrogObject *FrogErr_Get(void);

void FrogErr_Post(char *name, char *message);

void FrogErr_Operator(FrogObject *a, FrogObject *b, char *op);

#define FrogErr_Value(m) FrogErr_Post("ValueError", m)

#define FrogErr_Memory() FrogErr_Post("MemoryError", NULL)

#define FrogErr_Div0() FrogErr_Post("ArithmeticError", "Division or \
modulo by zero!")

#endif
