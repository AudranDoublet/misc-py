#ifndef __FROGOBJECTS_H__
#define __FROGOBJECTS_H__

typedef struct FrogObject FrogObject;
typedef struct FrogSObject FrogSObject;

typedef struct FrogType FrogType;

#define FrogObjHead FrogObject ob_base;
#define FrogSObjHead FrogSObject ob_base;

typedef FrogObject *(*unaryfunction)(FrogObject *);
typedef FrogObject *(*binaryfunction)(FrogObject *, FrogObject *);
typedef FrogObject *(*ternaryfunction)(FrogObject *, FrogObject *, FrogObject *);
typedef size_t (*sizefunction)(FrogObject *);
typedef int (*intfunction)(FrogObject *);
typedef int (*biintfunction)(FrogObject *, FrogObject *);
typedef long (*longfunction)(FrogObject *);

typedef FrogObject *(*getattrbfunction)(FrogObject *, char *);
typedef FrogObject *(*setattrbfunction)(FrogObject *, char *, FrogObject *);

typedef struct {
	binaryfunction add;
	binaryfunction sub;
	binaryfunction mul;
	binaryfunction div;
	binaryfunction divfloor;
	binaryfunction mod;
	binaryfunction pow;
	unaryfunction  neg;
	unaryfunction  pos;
	unaryfunction  abs;
	unaryfunction inv;
	binaryfunction lshift;
	binaryfunction rshift;
	binaryfunction and;
	binaryfunction or;
	binaryfunction xor;

	binaryfunction inplace_add;
	binaryfunction inplace_sub;
	binaryfunction inplace_mul;
	binaryfunction inplace_div;
	binaryfunction inplace_divfloor;
	binaryfunction inplace_mod;
	binaryfunction inplace_pow;
	unaryfunction  inplace_neg;
	unaryfunction  inplace_pos;
	unaryfunction  inplace_abs;
	binaryfunction inplace_inv;
	binaryfunction inplace_lshift;
	binaryfunction inplace_rshift;
	binaryfunction inplace_and;
	binaryfunction inplace_or;
	binaryfunction inplace_xor;
} FrogAsNumber;

typedef struct {
	sizefunction   size;
	binaryfunction set;
	unaryfunction  get;
} FrogAsSequence;

struct FrogObject {
	ssize_t refcnt;
	FrogType *type;
};

#define ObRefcnt(ob)	(((FrogObject *)ob)->refcnt)
#define ObType(ob)	(((FrogObject *)ob)->type)

struct FrogSObject {
	FrogObjHead
	ssize_t size;
};

#define ObSize(ob)	(((FrogSObject *)ob)->size)

struct FrogType {
	FrogObjHead
	char *name;

	getattrbfunction getter;
	setattrbfunction setter;

	longfunction hash;
	sizefunction size;

	unaryfunction tostr;
	unaryfunction toint;

	biintfunction compare;

	FrogAsNumber *as_number;
	FrogAsSequence *as_sequence;

	ternaryfunction *call;
};

FrogObject *ob_tostr(FrogObject *ob);

long ob_hash(FrogObject *ob);
int ob_compare(FrogObject *a, FrogObject b);

#endif
