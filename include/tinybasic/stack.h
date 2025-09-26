#pragma once

#include <stdint.h>

enum { STACK_SIZE = 16, };\

struct stack {
    uint8_t data[STACK_SIZE];
    int pos;
};

int stack_push(struct stack *stack, uint8_t value);
int stack_pop(struct stack *stack, uint8_t *value);