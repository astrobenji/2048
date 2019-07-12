#include <time.h>
#include <stdlib.h>
#include <assert.h>
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

move_t get_next_move(uint8_t board[SIZE][SIZE], int max_depth,
					 propagation_t propagation, out_data_t *out_data){
	
	move_t best_action, i;
	node_t *start, newNode;
	movelist_t *best_moves;
	
	node_t *explored[initial_size];
	int n_explored = 0, size_of_explored = initial_size;
	
	initialize_ai();
	
	start = malloc(sizeof(*start));
	assert(start != NULL);
	start->depth = 0;
	start->num_childs = 0;
	copyBoard(board, start->board);
	
	best_moves = malloc(sizeof(*best_moves));
	assert(best_moves != NULL);
	best_moves->n_poss_moves = 0;
	best_moves->top_score = 0;
	
	heap_push(&h, start);
	while(h.count) { // While the PQ is not empty:
		start = heap_delete(&h);
		out_data->n_exp++;
		
		// Add this node to the list of explored nodes
		if(n_explored == size_of_explored) {
			explored = realloc(explored, sizeof(explored)*2);
			assert(explored != NULL);
			size_of_explored *= 2;
		}
		explored[n_explored++] = start;
		
		if(start->depth < max_depth) {
			// Explore each possible move
			for(i=0; i<N_MOVES; i++) {
				newNode = applyAction(start, i);
				out_data->n_gen++;
				if(differentBoards(start->board, newNode->board)) {
					// Then this move is a valid move.
					heap_push(%h, newNode);
					findFirstAction(newNode, propogation, best_moves);
				} else {
					free(newNode);
				}
			}
		}
	}
	
	freeArray(explored, n_explored);
	best_action = (best_moves->moves)[rand()%(best_moves->n_poss_moves)];
	free(best_moves);
	return best_action;
}

void copyBoard(uint8_t oldBoard[SIZE][SIZE], uint8_t newBoard[SIZE][SIZE]) {
	int i,j;
	for(i=0; i<SIZE; i++) {
		for(j=0; j<size; j++) {
			newBoard[i][j] = oldBoard[i][j];
		}
	}
}

// Returns 1 if the boards are different and 0 if otherwise.
int differentBoards(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]) {
	int i, j;
	for(i=0; i<SIZE; i++) {
		for(j=0; j<size; j++) {
			if(board1[i][j] != board2[i][j]) return 1;
		}
	}
	return 0;
}

node_t *applyAction(node_t *oldNode, move_t direction) {
	// Malloc a new node.
	node_t *newNode;
	newNode = malloc(sizeof(*newNode));
	assert(newNode != NULL);
	// Fill in the relevant fields
	newNode->depth = oldNode->depth + 1;
	newNode->num_childs = 0;
	newNode->parent = oldNode;
	
	oldNode->move  = direction; // Tells us which direction we moved in
	
	copyBoard(oldNode->board, newNode->board);
	
	// Get the new board and the score from the relevant movement.
	if(direction == up) {
		moveUp(newNode->board, newNode->priority);
	} else if(direction == down) {
		moveDown(newNode->board, newNode->priority);
	} else if(direction == left) {
		moveLeft(newNode->board, newNode->priority);
	} else {
		moveRight(newNode->board, newNode->priority);
	}
	
	return newNode;
}

void findFirstAction(node_t *node, propogation_t propogation,
					 movelist_t *best_moves) {
	uint32_t path_score=0; // Average or Max of all scores.
	
	// Find the score for this path.
	while(newNode->depth >= 1) {
		if(propogation == max && newNode->priority > path_score) {
			path_score = newNode->priority;
		} else {
			path_score += newNode->priority;
		}
		newNode = newNode->parent;
	}
	
	if(path_score > best_moves->top_score) {
		// We have a new best path
		best_moves->top_score = path_score;
		(best_moves->moves)[0] = newNode->move; // The move we have just taken
	} else if(path_score == best_moves->top_score) {
		// Add the new move to the list of best moves if it is not already there
		int i;
		for(i=0; i<best_moves->n_poss_moves; i++) {
			if(newNode->move == (best_moves->moves)[i]) return;
		}
		(best_moves->moves)[(best_moves->n_poss_moves)++] = newNode->move;
	}
}
	   
void freeArray(node_t *nodes[] int n_nodes) {
	int i;
	for(i=0; i<n_nodes: i++) {
		free(nodes[i]);
	}
}
