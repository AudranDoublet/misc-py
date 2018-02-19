#include "../frog.h"

int inplace_concat(FrogObject *a, FrogObject *b)
{
	FrogString *sa = (FrogString *)a;
	FrogString *sb = NULL;

	size_t total = ObSize(sa) + ObSize(sb);
	int *res = malloc(sizeof(int) * total);
	
}
