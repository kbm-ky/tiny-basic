#pragma once

#include <tinybasic/node.h>

struct node_pool {
    struct node *data;
    int len;
    int cap;
};

// Initialize a node pool with capacity
int node_pool_init(struct node_pool *node_pool, int capacity);

// Destroy node pool
void node_pool_destroy(struct node_pool *node_pool);

// Push value of node into the node pull, returning a pointer or NULL
struct node *node_pool_push(struct node_pool *node_pool, struct node node);

// Reset the length of the node pool to zero
void node_pool_reset(struct node_pool *node_pool);