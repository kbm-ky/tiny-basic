#include <tinybasic/node_pool.h>
#include <tinybasic/node.h>
#include <tinybasic/common.h>

#include <stdio.h>
#include <stdlib.h>

int test_ok() {
    struct node_pool node_pool;
    int capacity = 4;
    int rc = node_pool_init(&node_pool, capacity);
    if (rc != RC_SUCCESS) {
        printf("test_ok: unable to init node_pool!\n");
        return 1;
    }

    struct node node = {};
    for (int i = 0; i < capacity; i++) {
        struct node *ptr = node_pool_push(&node_pool, node);
        if (ptr == NULL) {
            printf("test_ok[idx=%d]: failed to push node!", i);
            return 1;
        }
    }

    struct node *ptr = node_pool_push(&node_pool, node);
    if (ptr != NULL) {
        printf("test_ok: expected push to fail!\n");
        return 1;
    }

    node_pool_reset(&node_pool);

    node_pool_destroy(&node_pool);

    printf("test_ok: OK\n");
    return 0;
}

int main() {
    printf("node_pool_test: ...\n\n");
    int count = 0;
    count += test_ok();

    if (count > 0) {
        printf("\nnode_pool_test: %d test(s) failed!!!\n\n\n", count);
        return 1;
    }

    printf("\nnode_pool_test: All tests done.\n\n\n");
}