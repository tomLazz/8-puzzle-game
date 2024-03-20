#include "astarsolver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    Gameboard to_solve;
    int x, y, temp = 1, *moves;
    if(argc != 10)
        exit(1);

    init_board(&to_solve);

    for(x = 0; x < 3; x++) {
        for(y = 0; y < 3; y++) {
            to_solve.arr[x][y] = *argv[temp++] - '0';
        }
    }

    to_solve.h = manhattan_score(&to_solve);
    to_solve.f = to_solve.h + to_solve.g;

    moves = solve_with_a_star(&to_solve);

    while(*moves != 0){
        printf("%d\n", *moves++);
    }

    return 0;
}