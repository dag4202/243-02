//queueTest.c

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int main (void) {
	int *ptr = malloc(sizeof(int) * 20); 
	for (int i = 0; i<20; i++){
		ptr[i]=i;
	}
	Queue queue = que_create();
	printf("Push, Pop and Front:\n");
	for (int i = 0; i<20; i++){
		que_push((void *)&ptr[i], queue);
		printf("Pushed: %d\n", ptr[i]);
	}
	for(int i=0; i<20; i++){
		printf("Popped: %d\n",*((int *)que_pop(queue)));
	}
	que_push((void *)&ptr[0], queue);
	printf("Front and isEmpty:\n");
	printf("Front of queue (should be 0): %d\nc", *((int *)que_front(queue)));
	if (que_isEmpty(queue)){
		printf("Queue should not be empty. It is.\n");
	} else {
		printf("que_isEmpty succeeded\n");
	}
	printf("Clear and Destroy:\n");
	que_clear(queue);
	if (que_isEmpty(queue)){
		printf("isEmpty AND clear succeeded\n");

	} else {
		printf("isEmpty or clear Failed\n");

	}
	printf("Use Valgrind to check Destroy\n");
	que_destroy(queue);
	free(ptr);
	return 1;



}
