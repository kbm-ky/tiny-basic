#pragma once

#include <stdlib.h>

struct editor {
    char *line;
    size_t line_len;
};

struct editor editor_init(); 

void editor_destroy(struct editor *editor); 

int editor_prompt(struct editor *editor, char **string);

void editor_puts(struct editor *editor, char *string);