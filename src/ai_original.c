#include <time.h>
#include <stdlib.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"

struct heap h;

void initialize_ai(){
	heap_init(&h);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation ){
	move_t best_action = rand() % 4;

	//FILL IN THE GRAPH ALGORITHM
	
	return best_action;
}
