//dag4202

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#define HASH_ALLOC_UNIT 10


typedef struct h_node {
	void *key;
	void *data;
} HashNode;

struct hashTable {
	HashNode **content;
	unsigned int (*hash)(const void *key, unsigned int size);
	size_t capacity;
	size_t count;
};

typedef struct hashTable *HashTable;



#define _HASH_IMPL_
#include "hashTable.h"


HashTable ht_create(unsigned int (*hash)(const void *key, unsigned int size)){
	HashTable table = (HashTable) malloc(sizeof(struct hashTable));
	table->content = 0;
	table->hash = hash;
	table->capacity=0;
	table->count = 0;
	return table;


}

int ht_contains(void *key, HashTable table){
	if (table->content==0){
		return 0;
	}
	unsigned int (*hash)(const void *key, unsigned int size) = table->hash;
	unsigned int index = hash(key, table->capacity);
	unsigned int start = index;
	while(table->content[index]!=0){
                if(table->content[index]->key != key){
                        index = (index+1) % table->capacity;
                } else {
                        break;
                } 
                if (index==start){
                        return 0;
                } 
        }
	return table->content[index]!=0;


}

void ht_destroy(HashTable table){
	assert(table!=0);
	if(table->content!=0){
                for (unsigned int i = 0; i< table->capacity; i++){
                        if (table->content[i]!=0){
                                free(table->content[i]);
                        }
                }
                free(table->content);
        }

	free(table);


}
void ht_clear(HashTable table){
	assert(table!=0);
	if(table->content!=0){
		for (unsigned int i = 0; i< table->capacity; i++){
			if (table->content[i]!=0){
				free(table->content[i]);
			}
		}
		free(table->content);
		table->content = 0;
	}
	table->capacity = 0;
	table->count = 0;

}


void ht_add(void *key, void *data, HashTable table){
	assert(table!=0);
	unsigned int (*hash)(const void *key, unsigned int size) = table->hash;
	unsigned int index;
	if (table->content==0){
		table->content = calloc(HASH_ALLOC_UNIT, sizeof(HashNode*));
		table->capacity = HASH_ALLOC_UNIT;
	} else if (((double)table->count)/(table->capacity)>= 0.7){
		HashNode **tmp;
		tmp = calloc((table->capacity + HASH_ALLOC_UNIT),sizeof(HashNode*));
		for (unsigned int i = 0; i< table->capacity; i++){
			if (table->content[i]!=0){
				tmp[i] = table->content[i];
			}
		}
		free(table->content);
		table->content = tmp;
		table->capacity += HASH_ALLOC_UNIT;
	}
	index = hash(key, table->capacity);
	while(table->content[index]!=0){
		if (table->content[index]->key!=key){
			index = (index+1) % table->capacity;
		} else {
			break;
		}
	}
	if  (table->content[index] == 0){
		HashNode *node = malloc(sizeof(HashNode));
		node->key = key;
		node->data = data; 
		table->content[index] = node;
		table->count+=1;
	} else {
		table->content[index]->data = data;
	}
}


int ht_remove( void *key, HashTable table){
	assert(table!=0);
        unsigned int (*hash)(const void *key, unsigned int size) = table->hash;
        unsigned int index = hash(key, table->capacity);
        unsigned int start = index;
        while(table->content[index]!=0){
                if(table->content[index]->key != key){
                        index = (index+1) % table->capacity;
                } else {
                        break;
                }
                if (index==start){
                        return -1;
                } 
        }
        if (table->content[index] == 0){
                return -1;
        }
	free(table->content[index]);
	table->content[index] = 0;
	table->count -=1;
	return 1;
}


void *ht_get(void *key, HashTable table){
	assert(table!=0);
        unsigned int (*hash)(const void *key, unsigned int size)= table->hash;
        unsigned int index = hash(key, table->capacity);
	unsigned int start = index;
        while(table->content[index]!=0){
		if(table->content[index]->key != key){
                	index = (index+1) % table->capacity;
		} else {
			break;
		}
		if (index==start){
			return NULL;
		} 
        }
	if (table->content[index] == 0){
		return NULL;
	}
	return table->content[index]->data;
}

void **ht_keys(HashTable table){
	if (table->content == 0){
		return NULL;
	}
	void **keys = malloc(table->count * sizeof(void *));
	unsigned int count = 0;
	for (unsigned int i = 0; i< table->capacity; i++){
		if (table->content[i] != 0){
			keys[count] = table->content[i]->key;
			count+=1;
		}

	}
	return keys;

}
unsigned int ht_count(HashTable table){
	return (table->count);
}

unsigned int ht_capacity(HashTable table){
	return (table->capacity);
}
