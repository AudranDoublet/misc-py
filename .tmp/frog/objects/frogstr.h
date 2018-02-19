#ifndef __FROGOBJECTS_STR_H__
#define __FROGOBJECTS_STR_H__

typedef struct
{
	FrogObjHead
	size_t length;
	long hash;

	unsigned int *str;
} FrogString;

FrogString utf8toStr(char *utf8, char end, int escape);

FrogString 

#endif
