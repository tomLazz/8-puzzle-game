#if !defined(GAMEBOARD_H)
#define GAMEBOARD_H

/*moves 1 = up, 2 = down, 3 = left, 4 = right*/

/*finished board for comparison*/


typedef struct gameboard {
    int **arr;
    
    int g; /*number of moves*/
    int h; /*manhattan score*/
    int f; /*f = g + h*/
    int path_size;

    int *path;
    struct gameboard *next; /*in context of hash table*/
} Gameboard;

 
void init_board(Gameboard *board);

int *num_position(Gameboard *board, int num); /*returns coordinates of */

short is_complete(Gameboard *board);

int manhattan_score(Gameboard *board);

short move(Gameboard *board, short move); /*return 1 if move, 0 if illegal move*/

void scramble(Gameboard *board);

Gameboard *create_child(Gameboard *board, int move_);

int board_eql(Gameboard *first, Gameboard *second);

int *legal_moves(Gameboard *board);

void free_board(Gameboard *board); 

#endif