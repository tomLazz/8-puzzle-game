#include "set.h"
#include <stdlib.h>
#include <stdio.h>


unsigned long hash_board(Gameboard *board) {
    unsigned long to_return = 0;
    int i, j; 

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            to_return = (to_return << 4) | board->arr[i][j];
        }
    }

    return to_return;
}

void init_hash(Set *visited_set) {
    int i;
    visited_set->curr_size = 0;
    visited_set->max_size = 50000;

    visited_set->table = malloc(50000 * sizeof(Gameboard*));

    for(i = 0; i < visited_set->max_size; i++) {
        visited_set->table[i] = NULL;
    }
}

unsigned int contains(Set *visited_set, Gameboard *board) {
    Gameboard *curr;

    if(visited_set == NULL || board == NULL) 
        return 0;

    curr = visited_set->table[hash_board(board) % visited_set->max_size];

    while(curr != NULL && !board_eql(curr, board)) {
        curr = curr->next;
    }

    return (curr == NULL)? 0: 1;
}

void add(Set *visited_set, Gameboard *board) {
    int index;

    if(visited_set == NULL || board == NULL) 
        return;

    index = hash_board(board) % visited_set->max_size;

    if(visited_set->table[index] != NULL) {
        Gameboard *curr = visited_set->table[index], *next;
        next = curr->next;
        while(next != NULL) {
            curr = next;
            next = next->next;
        }
        
        curr->next = board;
    } else {
        visited_set->table[index] = board;
    }
}