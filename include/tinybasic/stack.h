#pragma once

#include <stdint.h>

enum { STACK_SIZE = 16, };\

struct stack {
    int data[STACK_SIZE];
    int pos;
};

int stack_push(struct stack *stack, int value);
int stack_pop(struct stack *stack, int *value);