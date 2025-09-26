#pragma once

#include <stdlib.h>

struct editor {
    char *line;
    size_t line_len;
};

struct editor editor_init(); 

void editor_destroy(struct editor *editor); 

int editor_prompt(struct editor *editor, char **string, char *format, ...);

void editor_println(struct editor *editor, char *string);

void editor_print(struct editor *editor, char *string);

int editor_printf(struct editor *editor, char *format, ...);