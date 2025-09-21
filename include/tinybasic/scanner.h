#pragma once

#include <stdint.h>

enum token_kind {
    TOKEN_UNDEFINED = 0,
    TOKEN_NUMBER,
    TOKEN_INVALID_NUMBER,


    TOKEN_EOF,
};

struct token {
    int pos;
    int len;
    enum token_kind kind;
};

struct scanner {
    const char *source;
    int pos;
    int mark;
};

struct scanner scanner_init(const char *source);
struct token scanner_next(struct scanner *scanner);