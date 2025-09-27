#pragma once

#include <tinybasic/common.h>

#include <stdint.h>
// #include <string.h>

enum {
    PROGRAM_ROWS = 256,
    PROGRAM_SIZE = PROGRAM_ROWS * MAX_INPUT_SIZE,
};

struct program {
    char *data;
};

// Initialize program memory
int program_init(struct program *program);

// Cleanup program memory
void program_destroy(struct program *program); 

// Write a line to the program
int program_write_line(struct program *program, uint8_t line, const char *data);

// Retrieve a line from the program
char *program_get_line(struct program *program, uint8_t line);

// Clear all program lines
void program_clear_all(struct program *program);

// Clear a single program line
void program_clear_line(struct program *program, uint8_t line);

