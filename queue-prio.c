#include <stdio.h>
#include <stdlib.h>
#include "queue-prio.h"

static void insert_helper(Queue *q, int size);
static void min_heapify(Queue *q, int index);

void init_queue(Queue *to_init) {
   to_init->array = calloc(sizeof(Gameboard*), MAX_SIZE);
   to_init->capacity = MAX_SIZE;
   to_init->size = 0;
}

void enqueue(Queue *q, Gameboard *to_add) {
    if(q->capacity <= q->size){
        return;
    }

    q->array[q->size] = to_add;
    insert_helper(q, q->size);
    q->size++;
}

static void insert_helper(Queue *q, int size) {
    int par = (size - 1) / 2;


    if(q->array[par]->f > q->array[size]->f) {

        Gameboard *temp = q->array[par];
        q->array[par] = q->array[size];
        q->array[size] = temp;

        insert_helper(q, par);
    }
}


Gameboard *dequeue(Queue *q) {
    if(q->size == 0) {
        return NULL;
    }
    Gameboard *to_return = q->array[0];
    q->array[0] = q->array[q->size - 1];
    q->array[q->size - 1] = NULL;
    q->size--;

    min_heapify(q, 0);

    return to_return;
}

static void min_heapify(Queue *q, int index) {
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int min = index;

    if (left >= q->size || left < 0)
        left = -1;
    if (right >= q->size || right < 0)
        right = -1;

    if (left != -1 && q->array[left]->f < q->array[min]->f)
        min = left;
    if (right != -1 && q->array[right]->f < q->array[min]->f)
        min = right;

    if(min != index) {
        Gameboard *temp = q->array[min];
        q->array[min] = q->array[index];
        q->array[index] = temp;

        min_heapify(q, min);
    }
}