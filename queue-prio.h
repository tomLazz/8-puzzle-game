
#if !defined(QUEUE_PRIO_H)
#define QUEUE_PRIO_H
#define MAX_SIZE 30000

#include "gameboard.h"


typedef struct queue {
    Gameboard **array;
    int capacity;
    int size;
} Queue;

void init_queue(Queue *to_init);
void enqueue(Queue *the_queue, Gameboard *to_add);
Gameboard *dequeue(Queue *q);

#endif
