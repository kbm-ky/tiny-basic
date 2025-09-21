#pragma once

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
    TOKEN_EQUAL,
    TOKEN_LT,
    TOKEN_LTE,
    TOKEN_GT,
    TOKEN_GTE,
    TOKEN_NEQUAL,
    TOKEN_COMMA,
    TOKEN_NEWLINE,
    TOKEN_STRING,
    TOKEN_INVALID_STRING,

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

// Constructs a scanner for the given source string
struct scanner scanner_init(const char *source);

// Returns the current token and advances to the next
struct token scanner_next(struct scanner *scanner);

// Returns the scanner's source string
const char *scanner_source(struct scanner *scanner); 