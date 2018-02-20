#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__
struct entry_s
{
	long hash;
	char *key;
	void *value;
	struct entry_s *next;
};

struct hashmap
{
	size_t size;
	size_t capacity;
	struct entry_s *entries;	
};


struct hashmap *create_hashmap();
#endif
