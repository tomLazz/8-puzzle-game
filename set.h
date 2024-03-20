#if !defined(SET_H)
#define SET_H


#include "gameboard.h"


typedef struct set {
    int max_size;
    int curr_size;
    Gameboard **table;
} Set;

unsigned long hash_board(Gameboard *board);

void init_hash(Set *visited_set);

unsigned int contains(Set *visited_set, Gameboard *board);

void add(Set *visited_set, Gameboard *board);

#endif