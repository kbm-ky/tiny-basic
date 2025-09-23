#include <tinybasic/node_pool.h>
#include <tinybasic/node.h>
#include <tinybasic/common.h>

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

// Initialize a node pool with capacity
int node_pool_init(struct node_pool *node_pool, int capacity) {
    assert(node_pool);
    assert(capacity > 0);

    struct node *data = calloc(capacity, sizeof(struct node));
    if (data == NULL) {
        return RC_ERR_UNABLE_TO_ALLOC;
    } 

    *node_pool = (struct node_pool){
        .data = data,
        .cap = capacity,
        .len = 0,
    };

    return RC_SUCCESS;
}

// Destroy node pool
void node_pool_destroy(struct node_pool *node_pool) {
    assert(node_pool);
    if (node_pool->data) {
        free(node_pool->data);
        node_pool->data = 0;
    }
}

// Checks if node pool is out of space
static bool _empty(struct node_pool *node_pool) {
    return node_pool->len >= node_pool->cap;
}

// Push value of node into the node pool, returning a pointer or NULL
struct node *node_pool_push(struct node_pool *node_pool, struct node node) {
    assert(node_pool);

    if (_empty(node_pool)) {
        return NULL;
    }

    struct node *next = &node_pool->data[node_pool->len++];
    *next = node;
    return next;
}

// Reset the length of the node pool to zero
void node_pool_reset(struct node_pool *node_pool) {
    assert(node_pool);
    node_pool->len = 0;
}