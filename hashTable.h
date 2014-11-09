//dag4202
#include <stddef.h>


#ifndef  _HASH_IMPL_
typedef struct Hash_Table{ } *HashTable;
#endif



HashTable ht_create(unsigned int (*hash)( const void *key, unsigned int size ) );

int ht_contains(void *key, HashTable table);

void ht_destroy(HashTable table);

void ht_clear(HashTable table);

void ht_add(void *key, void *data, HashTable table);

int ht_remove(void *key, HashTable table);

void *ht_get(void *key, HashTable table);

void **ht_keys(HashTable table);

unsigned int ht_count(HashTable table);

unsigned int ht_capacity(HashTable table);
