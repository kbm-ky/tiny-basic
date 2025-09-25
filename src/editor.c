#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <tinybasic/editor.h>
#include <tinybasic/common.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

enum { 
    MAX_BUFFER_SIZE = 80,
    MAX_INPUT_SIZE = 72,
    MAX_INPUT_STRLEN = MAX_BUFFER_SIZE -1,
};

struct editor editor_init() {
    return (struct editor) {};
}

void editor_destroy(struct editor *editor) {
    if (editor->line != NULL) {
        free(editor->line);
        editor->line = NULL;
    }
}

int editor_prompt(struct editor *editor, char **string) {
    printf("> ");
    ssize_t result = getline(&editor->line, &editor->line_len, stdin);
    if (result == -1) {
        return RC_ERR_UNABLE_TO_GETLINE;
    }
    if (strlen(editor->line) > MAX_INPUT_STRLEN) {
        return RC_ERR_INPUT_STRLEN_EXCEEDED;
    }
    *string = editor->line;
    return RC_SUCCESS;
}

void editor_println(struct editor *editor, char *string) {
    puts(string);
}

void editor_print(struct editor *editor, char *string) {
    printf("%s", string);
}

int editor_printf(struct editor *editor, char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);
    return ret;
}