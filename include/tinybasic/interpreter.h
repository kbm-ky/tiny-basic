#pragma once

#include <tinybasic/scanner.h>

struct interpreter {
    const char *source;
    struct scanner scanner;
    struct token current;
};


// Constructs an interpreter
struct interpreter interpreter_init(void);