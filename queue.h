//queue.h


#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifndef _QUEUE_IMPL_
typedef struct { } *Queue;
#endif

Queue que_create(void);
void que_destroy(Queue queue); 

void que_clear(Queue queue);

void que_push(void *data, Queue queue);

void *que_pop(Queue queue);

void *que_front(Queue queue);

int que_isEmpty(Queue queue);

#endif
