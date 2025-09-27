#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>

#include <tinybasic/common.h>
#include <tinybasic/interpreter.h>
#include <tinybasic/variables.h>
#include <tinybasic/editor.h>
#include <tinybasic/program.h>


int main() {
    // initialize editor and variable memory
    struct editor editor =  editor_init();
    struct variables variables = {};
    int rc = variables_init(&variables);
    if (rc != RC_SUCCESS) {
        printf("ERROR: Cannot create variables!\n");
        return 1;
    }

    // Create program space
    struct program program = {};
    rc = program_init(&program);
    if (rc != RC_SUCCESS) {
        printf("ERROR: Cannot create program!\n");
        return 1;
    }

    // Kick the direct mode interpreter
    struct interpreter interpreter = interpreter_init(&variables, &program);
    editor_printf(&editor, "Welcome to TINY-BASIC!\n\n");
    editor_printf(&editor, "Type BYE to exit interpreter.\n");
    rc = interpreter_loop(&interpreter, &editor);

    // Cleanup
    interpreter_destroy(&interpreter);
    variables_destroy(&variables);
    editor_destroy(&editor);

    return 0;
}