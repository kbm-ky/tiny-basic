#pragma once

#include <stdlib.h>

struct editor {
    char *line;
    size_t line_len;
};

// Initialize an editor structure
struct editor editor_init(); 

// Destroys editor
void editor_destroy(struct editor *editor); 

// Prompts the user for input
int editor_prompt(struct editor *editor, char **string, char *format, ...);

// Utility print line
void editor_println(struct editor *editor, char *string);

// Utility print
void editor_print(struct editor *editor, char *string);

// Utility printf
int editor_printf(struct editor *editor, char *format, ...);