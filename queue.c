#include <stdlib.h>
#include <assert.h>


#define QUEUE_ALLOC_UNIT 10

struct queueStruct {
	void **content;
	unsigned int capacity;
	unsigned int end;
	unsigned int start;
};

typedef struct queueStruct *Queue;

#define _QUEUE_IMPL_
#include "queue.h"

Queue que_create(void) {
	Queue queue;

	queue = (Queue) malloc(sizeof(struct queueStruct));
	queue->content = 0;
	queue->capacity = 0;
	queue->end = 0;
	queue->start = 0;
	return queue;
}

void que_destroy(Queue queue){
	assert(queue != 0);

	if(queue->content!=0) {
		free(queue->content);
	}

	free( queue );
}

void que_clear( Queue queue ){
	if (queue->content != 0) {
		free(queue->content);
		queue->content = 0;
	}
	queue->capacity = 0;
	queue->end = 0;
	queue->start = 0;
}

void que_push(void *data, Queue queue){
	assert(queue!=0);
	if (queue->content==0){
		queue->content = malloc(sizeof(void *) * QUEUE_ALLOC_UNIT);
		queue->capacity = QUEUE_ALLOC_UNIT;
	}
	if (queue->end >= queue->capacity){
		void *tmp;
		if (queue->start!=0){
			void **tempQueue = malloc(sizeof(void *) * queue->capacity);
			int count= 0;
			for (unsigned int i = queue->start; i< queue->end; i++){
				tmp = queue->content[i];
				tempQueue[count] = queue->content[i];
				count+=1;
			}
			free(queue->content);
			queue->content = tempQueue;
			queue->end -=queue->start;
			queue->start = 0;
		} else {
			tmp = realloc(queue->content, (queue->capacity + 
				QUEUE_ALLOC_UNIT) * sizeof(void *));
			queue->content = tmp;
			queue->capacity += QUEUE_ALLOC_UNIT;
		}
	}
	queue->content[queue->end] = data;
	queue->end += 1;
	
}

void *que_pop( Queue queue ) {
	assert((queue->end > 0) && (queue->start < queue->end));
	
	void *data = queue->content[queue->start++];
	if (queue->start == queue->end){
		queue->start=0;
		queue->end = 0;
	}
	return data;
}

void *que_front(Queue queue) {
	assert((queue->end > 0) && (queue->start < queue->end));
	
	return queue->content[queue->start];

}

int que_isEmpty(Queue queue){
	return (queue->start == queue->end);
}
