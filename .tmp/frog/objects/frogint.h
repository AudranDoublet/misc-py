#ifndef __FROGOBJECTS_INT_H__
#define __FROGOBJECTS_INT_H__

#define FIValue(v) (((FrogInt *)v)->value)
typedef struct {
	FrogObjHead
	long value;
} FrogInt;

#endif
