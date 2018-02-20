#ifndef __PARSER_H__
#define __PARSER_H__

#define TOKEN_ERROR		-1
#define TOKEN_EOF		0x00

// Values
#define TOKEN_INTEGER	0x01 // 10 0xaa
#define TOKEN_DECIMAL	0x02 // 0.10 0.10e10
#define TOKEN_STRING	0x03 // "example" 'example' @"example\:p"
#define TOKEN_TRUE	0x04 // True
#define TOKEN_FALSE	0x05 // False
#define TOKEN_NULL	0x06 // None
#define TOKEN_ID	0x07 // Identifier

// Branchement instructions
#define TOKEN_IF	0x10 // if
#define TOKEN_ELSE	0x11 // else
#define TOKEN_WHILE	0x12 // while
#define TOKEN_FOR	0x13 // for
#define TOKEN_FOREACH	0x14 // foreach
#define TOKEN_DO	0x15 // do
#define TOKEN_CONTINUE	0x16 // continue
#define TOKEN_BREAK	0x17 // break
#define TOKEN_TRY	0x18 // try
#define TOKEN_CATCH	0x19 // catch

// Functions instructions
#define TOKEN_RETURN	0x20 // return
#define TOKEN_YIELD	0x21 // yield
#define TOKEN_FUNCTION	0x22 // func
#define TOKEN_STATIC	0x23 // static
#define TOKEN_ABSTRACT	0x24 // abstract
#define TOKEN_PRIVATE	0x25 // private ?
#define TOKEN_PROTECTED 0x26 // protected ?

// Classes instruction
#define TOKEN_CLASS	0x30

//  General instructions
#define TOKEN_INCLUDE	0x40
#define TOKEN_OPEN	0x41 // {
#define TOKEN_CLOSE	0x42 // }
#define TOKEN_SOPEN	0x43 // (
#define TOKEN_SCLOSE	0x44 // )
#define TOKEN_IEND	0x45 // ;

// Comparaison operators
#define TOKEN_CMPEQ	0x50 // ==
#define TOKEN_CMPNE	0x51 // !=
#define TOKEN_CMPLO	0x52 // <
#define TOKEN_CMPGT	0x53 // >
#define TOKEN_CMPLE	0x54 // <=
#define TOKEN_CMPGE	0x56 // >=

// Operators
#define TOKEN_OPTPLS	0x60 // +
#define TOKEN_OPTMIN	0x61 // -
#define TOKEN_OPTMUL	0x62 // *
#define TOKEN_OPTDIV	0x63 // /
#define TOKEN_OPTMOD	0x64 // %
#define TOKEN_OPTPOW	0x65 // **
#define TOKEN_OPTNOT	0x66 // !

// Bitwise operators
#define TOKEN_BWOLSF	0x70 // <<
#define TOKEN_BWORSF	0x71 // >>
#define TOKEN_BWOOR	0x72 // |
#define TOKEN_BWOAND	0x73 // &
#define TOKEN_BWOXOR	0x74 // ^
#define TOKEN_BWONOT	0x75 // ~

typedef struct tokenizer tokenizer;
typedef fchar *(*nextline)(tokenizer *ptr);

typedef struct
{
	int type;
	FrogObject *value;
} tokeninfo;

struct automatanode
{
	struct automatanode *nexts[128];
	int tokentype;
};

struct tokenizer
{
	fchar *line;
	fchar *linepos;

	void *file;
	nextline linegetter;

	struct automatanode *head;
	tokeninfo *current;
	tokeninfo *next;
};

/**
 * Create a new tokenizer
 */
tokenizer *create_tokenizer(nextline linegetter, void *file);

/**
 * Change value of tokenizer->current (return the newvalue) and set tokenizer->next to NULL.
 */
tokeninfo *next_token(tokenizer *tkz);

/**
 * Change value of tokenizer->next and return the new value
 */
tokeninfo *peek_token(tokenizer *tkz);

/**
 * Verify that the result of next_token is of the right
 * token type.
 */
int eat_token(tokenizer *tkz, int type);

/**
 * Free the given parser
 */
void free_tokenizer(tokenizer *tkz);
#endif
