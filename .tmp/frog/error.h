#ifndef __FROGERROR_H__
#define __FROGERROR_H__

void FrogErr_Post(char *name, char *message);

void FrogErr_Compare(FrogObject *a, FrogObject *b);

void FrogErr_Operator(FrogObject *a, FrogObject *b, char *op);

#define FrogErr_Value(m) FrogErr_Post("ValueError", m)

#define FrogErr_Memory() FrogErr_Post("MemoryError", NULL)

#define FrogErr_Div0() FrogErr_Post("ArithmeticError", "Division or \
modulo by zero!")

#endif
