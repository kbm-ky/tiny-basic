#include <tinybasic/stack.h>
#include <tinybasic/common.h>

#include <stdint.h>

int stack_push(struct stack *stack, int value) {
    if (stack->pos >= STACK_SIZE) {
        return RC_ERR_STACK_OVERFLOW;
    }

    stack->data[stack->pos] = value;
    stack->pos++;
    return RC_SUCCESS;
}

int stack_pop(struct stack *stack, int *value) {
    if (stack->pos <= 0) {
        return RC_ERR_STACK_UNDERFLOW;
    }

    *value = stack->data[--stack->pos];
    return RC_SUCCESS;
}