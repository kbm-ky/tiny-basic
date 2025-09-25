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
    struct interpreter interpreter = interpreter_init();

    printf("Welcome to TINY-BASIC!\n");

    while (1) {
        char *line = NULL;
        int rc = editor_prompt(&editor, &line);
        if (rc == RC_ERR_INPUT_STRLEN_EXCEEDED) {
            printf("Input string length exceeded.  Retry...\n\n");
            continue;
        } else if (rc != RC_SUCCESS) {
            printf("Unrecoverable error\n\n");
            return 1;
        }

        // int rc = interpreter_interpret_line(&interpreter, &editor);
        

    }

    editor_destroy(&editor);

    return 0;
}