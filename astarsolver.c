#include "gameboard.h"
#include "queue-prio.h"
#include "set.h"

#include <stdio.h>
#include <stdlib.h>

/*recursive function to get all moves given pointer to complete board*/




int *solve_with_a_star(Gameboard *board) {
    Queue *queue = malloc(sizeof(Queue));
    Set *visited_set = malloc(sizeof(Set));

    init_queue(queue);
    init_hash(visited_set);

    enqueue(queue, board);

    while(queue->size != 0) {
        Gameboard *curr = dequeue(queue);

        if(is_complete(curr) != 0) {
            int *to_return = malloc((curr->path_size + 1) * sizeof(int)),
            *temp = curr->path, *to_copy = to_return;
 
            while(*temp != 0) { 
                *to_return++ = *temp++;
            }                                                                   
            to_return[0] = 0;
 
            free(visited_set->table);
            free(visited_set);

            while(queue->size != 0) {
                curr = dequeue(queue); 
                free_board(curr);
            }

            free(queue->array);
            free(queue);

            return to_copy;   
        } else {
            int i, *moves;
            moves = legal_moves(curr);
            add(visited_set, curr);
            
            for(i = 0; i < 4; i++) {
                if(moves[i] != 0) {
                    Gameboard *to_check = create_child(curr, moves[i]);

                    if(contains(visited_set, to_check) == 0) 
                        enqueue(queue, to_check);
                    else
                        free_board(to_check);
                    
                }
            }

        }
    }

    return NULL;
    
} 