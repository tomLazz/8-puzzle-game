#include "astarsolver.h"
#include "queue-prio.h"
#include "set.h"
#include "gameboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int boards_buffer[9 * 40000];
int moves_buffer[40000];

int main(void) {
    int i, num = 0, moves_fd, boards_fd;

    for(i = 0; i < 2000; i++) {
        Gameboard *board = malloc(sizeof(Gameboard));
        int *moves, *to_free_moves;
        init_board(board);
        scramble(board);
        to_free_moves = moves = solve_with_a_star(board);

        while(*moves != 0 && num < 40000) {
            int j, k, temp = 0;
            moves_buffer[num] = *moves;
            
            for(j = 0; j < 3; j++) { 
                for(k = 0; k < 3; k++) {
                    boards_buffer[9 * num + (temp++)] = board->arr[j][k];
                }
            }
            
            move(board, *moves++); 
            num++;
         
        }

        free_board(board);
        free(to_free_moves);
        
    }

    moves_fd = open("moves-data-40k.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    boards_fd = open("boards-data-40k.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    write(moves_fd, moves_buffer, sizeof(int) * 40000);
    write(boards_fd, boards_buffer, sizeof(int) * 40000 * 9);


    return 0;
}
