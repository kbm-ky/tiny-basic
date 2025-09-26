#pragma once

#include <tinybasic/scanner.h>
#include <tinybasic/editor.h>
#include <tinybasic/variables.h>
#include <tinybasic/stack.h>
#include <tinybasic/program.h>

#include <stdbool.h>

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
    int return_address;
    bool continuation;
    int pc;
    struct stack stack;
    struct program *program;
};


// Constructs an interpreter
struct interpreter interpreter_init(struct variables *variables, struct program *program);

// Cleans up interpreter
void interpreter_destroy(struct interpreter *interpreter);

// Main interpreter loop
int interpreter_loop(struct interpreter *interpreter, struct editor *editor);