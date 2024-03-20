#include "gameboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int finished[3][3] = {{1,2,3},{4,5,6},{7,8,0}};

void init_board(Gameboard *board) {
    int i, j;
    board->arr = malloc(3 * sizeof(int*));
    
    for(i = 0; i < 3; i++) {
        board->arr[i] = malloc(3 * sizeof(int));
        
    }

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            board->arr[i][j] = finished[i][j];
        }
    }

    board->h = 0;
    board->g = 0;
    board->f = board->g + board->h;
    board->path_size = 0;

    board->next = NULL;
    board->path = NULL;

}

/*return y,x coordinates*/
int *num_position(Gameboard *board, int num) {
    int *to_return;
    int i,j;

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(board->arr[i][j] == num) {
                to_return = malloc(sizeof(int[2]));
                to_return[0] = i;
                to_return[1] = j;

                return to_return;
            }
        }
    }

    return NULL;
}

short is_complete(Gameboard *board) {
    int i,j;

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(board->arr[i][j] != finished[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

int manhattan_score(Gameboard *board) {
    int total_displacement, num, y, x, *values;
     
    total_displacement = 0;
    num = 1;
    for(y = 0; y < 3; y++) {
        for(x = 0; x < 3; x++) {
            if(x == 2 && y == 2){
                values = num_position(board, 0);
            } else {
                values = num_position(board, num++);
            }

            total_displacement += abs((y - values[0])) + abs((x - values[1]));
        }
    }

    return total_displacement;
}

short move(Gameboard *board, short move) {
    int to_return = 1;
    int *values = num_position(board, 0);

    if(move == 1 && values[0] != 0) { /*move up and not in top row*/
        board->arr[values[0]][values[1]] = board->arr[values[0] - 1][values[1]];
        board->arr[values[0] - 1][values[1]] = 0;
    } else if(move == 2 && values[0] != 2) { /*move down and not in bottom row*/
        board->arr[values[0]][values[1]] = board->arr[values[0] + 1][values[1]];
        board->arr[values[0] + 1][values[1]] = 0;
    } else if(move == 3 && values[1] != 0) { /*move left and not in left row*/
        board->arr[values[0]][values[1]] = board->arr[values[0]][values[1] - 1];
        board->arr[values[0]][values[1] - 1] = 0;
    } else if(move == 4 && values[1] != 2) { /*move down and not in bottom row*/
        board->arr[values[0]][values[1]] = board->arr[values[0]][values[1] + 1];
        board->arr[values[0]][values[1] + 1] = 0;
    } else {
        to_return = 0;
    }

    return to_return;
}

static int is_solvable(int arr[]) {
    int inversions = 0, i, j;

    for (i = 0; i < 9 - 1; i++)
        for (j = i+1; j < 9; j++)
             if (arr[j] && arr[i] &&  arr[i] > arr[j])
                  inversions++;


    return (inversions % 2 == 0);
}

void scramble(Gameboard *board) {
    int i, j, temp = 1, index = 0;
    int arr[] = {1,2,3,4,5,6,7,8,0};

    while(temp) {
        for(i = 8; i > 0; i--) {
            int temp = arr[i];
            j = rand() % (i+1);
            arr[i] = arr[j];
            arr[j] = temp;
        }
        
        if(is_solvable(arr))
            temp = 0;
    }

   for(i = 0; i < 3; i++) {
            for(j = 0; j < 3; j++) {
                board->arr[i][j] = arr[index++];
        }
    }

    board->h = manhattan_score(board);
    board->g = 0;
    board->f = board->g + board->h;

}


Gameboard *create_child(Gameboard *board, int move_) {
    Gameboard *child = malloc(sizeof(Gameboard));
    init_board(child);
    int i,j, *par_temp, *chil_temp;

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            child->arr[i][j] = board->arr[i][j];
        }
    }

    move(child, move_);
    child->path_size = board->path_size + 1;

    

    if(board->path != NULL) {

        child->path = malloc((child->path_size + 1) * sizeof(int));
        par_temp = board->path;

        chil_temp = child->path;

        while(*par_temp != 0) 
            *chil_temp++ = *par_temp++;
         
        chil_temp[0] = move_;
        chil_temp[1] = 0;
    } else {
        child->path = malloc(2 * sizeof(int));
        child->path[0] = move_;
        child->path[1] = 0;

    }

    child->h = manhattan_score(child); 
    child->g = board->g + 1;
    child->f = child->g + child->h;
   
    return child;
}


int board_eql(Gameboard *first, Gameboard *second) {
    int i,j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(first->arr[i][j] != second->arr[i][j])
                return 0;
        }   
    }

    return 1;
    
}

int *legal_moves(Gameboard *board) {
    int *empty_pos = num_position(board, 0);
    int *to_return = malloc(4 * sizeof(int));
    to_return[0] = 1;
    to_return[1] = 2;
    to_return[2] = 3;
    to_return[3] = 4; 


    if(empty_pos[0] == 2)
        to_return[1] = 0;
    else if(empty_pos[0] == 0)
        to_return[0] = 0;

    if(empty_pos[1] == 2) 
        to_return[3] = 0;
    else if (empty_pos[1] == 0)
        to_return[2] = 0;
    

    return to_return;
    
}

void free_board(Gameboard *board) {
    free(board->arr[0]);
    free(board->arr[1]);
    free(board->arr[2]);
    free(board->arr);

    free(board->path);
    free(board);
}
