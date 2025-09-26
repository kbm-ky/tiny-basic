#pragma once

#include <tinybasic/scanner.h>
#include <tinybasic/editor.h>
#include <tinybasic/variables.h>

enum interpreter_mode {
    INTERPRETER_MODE_DIRECT,
    INTERPRETER_MODE_INDIRECT,
};


struct interpreter {
    const char *source;
    struct scanner scanner;
    struct token current;
    struct variables *variables;
    enum interpreter_mode mode;
};


// Constructs an interpreter
struct interpreter interpreter_init(struct variables *variables);

void interpreter_destroy(struct interpreter *interpreter);

int interpreter_loop(struct interpreter *interpreter, struct editor *editor);