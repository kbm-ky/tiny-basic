#pragma once

#include <tinybasic/scanner.h>
#include <tinybasic/editor.h>

struct interpreter {
    const char *source;
    struct scanner scanner;
    struct token current;
};


// Constructs an interpreter
struct interpreter interpreter_init(void);

int interpreter_loop(struct interpreter *interpreter, struct editor *editor);