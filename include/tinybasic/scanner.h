#pragma once

#include <stdint.h>

enum token_kind {
    TOKEN_UNDEFINED = 0,
    TOKEN_NUMBER,
    TOKEN_INVALID_NUMBER,
    TOKEN_PRINT,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_GOTO,
    TOKEN_INPUT,
    TOKEN_LET,
    TOKEN_GOSUB,
    TOKEN_RETURN,
    TOKEN_CLEAR,
    TOKEN_LIST,
    TOKEN_RUN,
    TOKEN_END,
    TOKEN_INVALID_IDENTIFIER,
    TOKEN_VARIABLE,



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