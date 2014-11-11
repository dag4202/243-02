#include "hashTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

unsigned int basicHash(const void *key, unsigned int size){
	return (*((int *)key) % 8) % size;

}


int main (void){
	HashTable table = ht_create(basicHash);
	int *ptr1;
	ptr1 = malloc(sizeof(int) * 20);
	for (int j = 0; j<20; j++){
		ptr1[j]=j;
	}
	int p;
	printf("ADD AND GET:\n");
	for(int k = 0; k<20;k++){
		if (k+1!=20){
			ht_add((void *) &ptr1[k], (void *) &ptr1[k+1], table);
		} else {
			ht_add( (void *) &ptr1[k], (void *) &ptr1[0], table);
		}
		p = *((int *)ht_get((void *)&ptr1[k], table));
		printf("Key: %d, Data: %d\n", ptr1[k],p); 
		k++;
	}
	if (ht_remove((void *) &ptr1[0], table)){
		printf("Removed Key: %d\n", ptr1[0]);
	}
	printf("KEYS:\n");
	int **keys = (int **) ht_keys(table);
	for (unsigned int i=0; i< ht_count(table); i++){
		printf("key #%d: %d\n",i, *keys[i]);
	}
	printf("Capacity: %d\n", ht_capacity(table));
	printf("Count:%d\n", ht_count(table));
	int *ptr2 = malloc(sizeof(int));
	*ptr2 = 8;
	if(ht_contains((void *)keys[0], table) && !ht_contains((void *)ptr2, table)){
		printf("ht_contains successful\n"); 
	}
	ht_clear(table);
	free(keys);
	keys = (int **) ht_keys(table);
	if (keys==NULL){
		printf("Successfully Cleared\n");
	}
	ht_destroy(table);
	free(ptr1);
	free(ptr2);
	
	return 1;



}
