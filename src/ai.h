#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"

#define N_MOVES 4
#define BOTTOM 3
#define RIGHT 3

/* An auxillery structure that holds a list of the best first moves our program 
 * can make.
 */
typedef struct {
	move_t moves[N_MOVES];
	int n_poss_moves;
	uint32_t top_score;
} movelist_t;


void initialize_ai();

move_t get_next_move(uint8_t board[SIZE][SIZE],int max_depth,
					 propagation_t propagation, out_data_t *out_data);

// Copy the contents of one array into another array
void copyBoard(uint8_t oldBoard[SIZE][SIZE], uint8_t newBoard[SIZE][SIZE]);

// Returns 1 if the boards are different and 0 if otherwise.
int differentBoards(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);

// Generate a new node by stepping in a certain direction
node_t *applyAction(node_t *oldNode, move_t direction);

/* Decide whether or not the first action made to get to this node gets is one 
 * of the best possible first moves. If it is, add it to the list of best moves.
 *
 * If the "max" propogation type is used, then the best path is chosen by 
 * finding the highest node in that path. If "avg" is used, then the best path
 * is the path with the highest average score along its nodes.
 */
movelist_t *findFirstAction(node_t *node, propagation_t propagation,
					 movelist_t *best_moves);

// Free all memory in an array, then free the array itself.
void freeArray(node_t *nodes[], int n_nodes);

// A function that initialises our movelist_t structure.
void default_moves(movelist_t *moves);

// Alter the priority of the node according to a set of rules.
void use_heuristics(node_t *node);

// Add a bonus if each column is monotonically increasing
void monotonic_cols(node_t *node);
#endif
