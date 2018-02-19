#ifndef __STRUTILS_H__
#define __STRUTILS_H__
/**
 * represent a string builder
 * to see how you can use it, see above functions
 * a string builder can be used to concatenates string more
 * efficiencly
 */
struct strbuilder
{
	size_t length;
	void *first;
	void *last;
	void *reserve;
};

/**
 * create an new strbuilder
 */
struct strbuilder *create_strbuilder(void);

/**
 * clear the strbuilder to use it another time without
 * reallocating memory
 */
void clear_strbuilder(struct strbuilder *builder);

/**
 * add the given string to the builder
 */
void append_strbuilder(struct strbuilder *builder, char *str);

/**
 * add the given character to the builder
 */
void add_strbuilder(struct strbuilder *builder, char chr);

/**
 * get the current value in a unique string
 */
char *tostr_strbuilder(struct strbuilder *builder);

/**
 * free the given builder
 */
void free_strbuilder(struct strbuilder *builder);
#endif
