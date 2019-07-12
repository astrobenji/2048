#ifndef __NODE__
#define __NODE__

#include "utils.h"

/**
 * Data structure containing the node information
 */
struct node_s{
    uint32_t priority; // The score of this node plus any bonuses
	uint32_t bonus;
    int depth; // The number of steps taken to reach this node
    move_t move;
    uint8_t board[SIZE][SIZE];
    struct node_s* parent;
};

typedef struct node_s node_t;

typedef struct{
	int n_gen;
	int n_exp;
}out_data_t;

#endif
