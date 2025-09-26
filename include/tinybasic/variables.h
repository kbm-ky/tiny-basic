#pragma once

#include <stdint.h>

struct variables {
    int16_t *data;
    uint32_t defined;
};

// Initialize variables
int variables_init(struct variables *variables); 
// Destroy variables
void variables_destroy(struct variables *variables);
// Set variable value
int variables_set(struct variables *variables, char name, int16_t value);
// Get variable value
int variables_get(struct variables *variables, char name, int16_t *value);