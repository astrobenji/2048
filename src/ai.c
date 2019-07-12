/* ai.c
*
* All code not provided in the base code was written by me.
*
* Created by Benjamin Metha, 681260
* methab@student.unimelb.edu.au
*
* Last updated October 14 2016.
*/


#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
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
	node_t *start, *tempNode;
	movelist_t *best_moves;
	
	node_t **explored;
	int n_explored = 0, size_of_explored = initial_size;
	
	start = malloc(sizeof(*start));
	assert(start != NULL);
	start->depth = 0;
	start->priority = 0;
	start->bonus = 0;
	copyBoard(board, start->board);
	
	best_moves = malloc(sizeof(*best_moves));
	assert(best_moves != NULL);
	default_moves(best_moves);
	
	explored = malloc(initial_size * sizeof(*explored));
	assert(explored != NULL);
	
	heap_push(&h, start);
	while(h.count) { // While the PQ is not empty:
		start = heap_delete(&h);
		
		// Add this node to the list of explored nodes
		if(n_explored == size_of_explored) {
			size_of_explored *= 2;
			explored = realloc(explored, sizeof(*explored)*size_of_explored);
			assert(explored != NULL);
		}
		explored[n_explored++] = start;
		
		if(start->depth < max_depth) {
			// Explore each possible move
			for(i=left; i<=down; i++) {
				tempNode = applyAction(start, i);
				out_data->n_gen++;
				if(differentBoards(start->board, tempNode->board)) {
					// Then this move is a valid move.
					heap_push(&h, tempNode);
					best_moves = findFirstAction(tempNode, propagation,
												 best_moves);
				} else {
					free(tempNode);
				}
			}
		}
	}
	
	out_data->n_exp += n_explored;
	
	// Free all memory allocated
	freeArray(explored, n_explored);
	
	// Randomly select a move from the list of best moves
	best_action = (best_moves->moves)[rand()%(best_moves->n_poss_moves)];
	
	free(best_moves);
	return best_action;
}

void copyBoard(uint8_t oldBoard[SIZE][SIZE], uint8_t newBoard[SIZE][SIZE]) {
	int i,j;
	for(i=0; i<SIZE; i++) {
		for(j=0; j<SIZE; j++) {
			newBoard[i][j] = oldBoard[i][j];
		}
	}
}

// Returns 1 if the boards are different and 0 if otherwise.
int differentBoards(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]) {
	int i, j;
	for(i=0; i<SIZE; i++) {
		for(j=0; j<SIZE; j++) {
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
	newNode->parent = oldNode;
	newNode->priority = oldNode->priority - oldNode->bonus;
	newNode->move = direction; // Tells us which direction we moved in
	newNode->bonus = 0;
	
	copyBoard(oldNode->board, newNode->board);
	
	// Get the new board and the score from the relevant movement.
	execute_move_t(newNode->board, &newNode->priority, direction);
	
	// Change the priority of this node according to our heuristics.
	use_heuristics(newNode);
	
	return newNode;
}

movelist_t *findFirstAction(node_t *node, propagation_t propagation,
							movelist_t *best_moves) {
	uint32_t path_score = 0; // Average or Max of all scores.
	move_t first_move = rand()%N_MOVES; // The first move that was taken
	int depth = node->depth;
	
	// Find the score for this path.
	while(node->depth >= 1) {
		if(propagation == max && node->priority > path_score) {
			path_score = node->priority;
		} else if(propagation == avg) {
			path_score += node->priority;
		}
		if(node->depth==1) first_move = node->move;
		node = node->parent;
	}
	
	if(propagation == avg && depth != 0) path_score /= depth;
	// We do not mind about integer division; near enough is good enough
	
	if(path_score > best_moves->top_score) {
		// We have a new best path
		best_moves->top_score = path_score;
		(best_moves->moves)[0] = first_move; // The move we have just taken
		best_moves->n_poss_moves = 1;
	} else if(path_score == best_moves->top_score) {
		// Add the new move to the list of best moves if it is not already there
		int i;
		for(i=0; i<best_moves->n_poss_moves; i++) {
			if(first_move == (best_moves->moves)[i]) return best_moves;
		}
		(best_moves->moves)[(best_moves->n_poss_moves)++] = first_move;
	}
	return best_moves;
}
	   
void freeArray(node_t *nodes[], int n_nodes) {
	int i;
	for(i=0; i<n_nodes; i++) {
		if(nodes[i]) free(nodes[i]);
	}
	free(nodes);
}

void assign_node(node_t *old_node, node_t *new_node) {
	new_node->priority = old_node->priority;
	new_node->depth = old_node->depth;
	new_node->move = old_node->move;
	new_node->parent = old_node->parent;
	copyBoard(old_node->board, new_node->board);
}

/* Initialise the movelist to contain every move, with a top score of 0.
 * This is to ensure that if a depth of 0 or less is used, the AI still
 * functions, randomly choosing moves.
 */
void default_moves(movelist_t *moves) {
	moves->n_poss_moves = N_MOVES;
	moves->top_score = 0;
	moves->moves[0] = left;
	moves->moves[1] = right;
	moves->moves[2] = up;
	moves->moves[3] = down;
}

// Alter the priority of the node according to a set of rules.
void use_heuristics(node_t *node) {
	int i,j;
	int n_empty_squares = 0;
	
	// Keep as many squares empty as possible!
	for(i=0;i<SIZE; i++) {
		for(j=0;j<SIZE; j++) {
			if(getTile(node->board, i, j) == 0) {
				node->bonus += maxTile(node->board);
				n_empty_squares++;
			}
		}
	}
	
	// Add a big bonus for having large tiles in the bottom corner
	node->bonus += 16*getTile(node->board, BOTTOM, RIGHT);
	
	//Add a smaller bonus for keeping large tiles in the bottom row
	for(i=0;i<RIGHT;i++) {
		node->bonus += 8*getTile(node->board, BOTTOM, i);
	}
	
	// Add a bonus for keeping sections that increase along a direction
	for(i=0; i<SIZE; i++) {
		// Check for each column; then rotate the board and repeat for each
		// direction
		monotonic_cols(node);
		rotateBoard(node->board);
	}
	
	node->priority += node->bonus;
	return;
}

void monotonic_cols(node_t *node) {
	int i,j;
	for(i=0; i<SIZE; i++) {
		for(j=0; j<BOTTOM; j++) {
			if(getTile(node->board, j, i) <= getTile(node->board, j+1, i)) {
				node->bonus += getTile(node->board, j, i)/16;
			} else {
				break;
			}
		}
	}
	return;
}
