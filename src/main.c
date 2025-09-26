#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tinybasic/common.h>
#include <tinybasic/interpreter.h>
#include <tinybasic/scanner.h>
#include <tinybasic/variables.h>
#include <tinybasic/editor.h>


int main() {
    struct editor editor =  editor_init();
    struct variables variables = {};
    int rc = variables_init(&variables);
    if (rc != RC_SUCCESS) {
        printf("ERROR: Cannot create variables!\n");
        return 1;
    }

    struct interpreter interpreter = interpreter_init(&variables);

    editor_printf(&editor, "Welcome to TINY-BASIC!\n");
    rc = interpreter_loop(&interpreter, &editor);

    interpreter_destroy(&interpreter);
    variables_destroy(&variables);
    editor_destroy(&editor);

    return 0;
}