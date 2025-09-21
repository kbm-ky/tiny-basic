#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

bool token_equals(struct token a, struct token b) {
    return  (a.pos == b.pos) &&
            (a.len == b.len) &&   
            (a.kind == b.kind);
}

struct scanner {
    const char *source;
    int pos;
    int mark;
};

bool _empty(struct scanner *scanner) {
    return scanner->pos >= strlen(scanner->source);
}

int _mark(struct scanner *scanner) {
    return (scanner->mark = scanner->pos);
}

char _peek(struct scanner *scanner) {
    return scanner->source[scanner->pos];
}

char _next(struct scanner *scanner) {
    return scanner->source[scanner->pos++];
}

void _skipwhitespace(struct scanner *scanner) {
    while (!_empty(scanner)) {
        char ch = _peek(scanner);
        if (!isspace(ch)) return;
        _next(scanner);
    }
}

struct token _make_token(struct scanner *scanner, enum token_kind kind) {
    int len = scanner->pos - scanner->mark;
    return (struct token) {
        .pos = scanner->mark,
        .len = len,
        .kind = kind,
    };
}

struct token _number(struct scanner *scanner) {
    _next(scanner); //eat digit from scan_next

    while (!_empty(scanner)) {
        char ch = _peek(scanner);
        if (!isdigit(ch)) break;
        _next(scanner);
    }

    //number cannot end with a letter, e.g. 100ABC
    if (!_empty(scanner) && isalpha(_peek(scanner))) {
        return _make_token(scanner, TOKEN_INVALID_NUMBER);
    }

    return _make_token(scanner, TOKEN_NUMBER);
}

struct scanner scanner_init(const char *source) {
    return (struct scanner){
        .source = source,
        .pos = 0,
        .mark = 0,
    };
}

struct token scanner_next(struct scanner *scanner) {
    _skipwhitespace(scanner);

    if (_empty(scanner)) {
        return _make_token(scanner, TOKEN_EOF);
    };

    char ch = _peek(scanner);
    _mark(scanner);

    if (isdigit(ch)) {
        return _number(scanner);
    } else {
        _next(scanner);
        return _make_token(scanner, TOKEN_UNDEFINED);
    }
}

struct test_digit {
    char *input;
    struct token want;
};

void test_scan_digit() {

    struct test_digit tests[] = {
        { //0
            .input = "0",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_NUMBER,
            },
        },
        { //1
            .input = "  0",
            .want = {
                .pos = 2,
                .len = 1,
                .kind = TOKEN_NUMBER,
            },
        },
        { //2
            .input = "10",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_NUMBER,
            },
        },
        { //3
            .input = "10 ",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_NUMBER,
            },
        },
        { //4
            .input = "10IF",
            .want = {
                .pos = 0,
                .len = 3,
                .kind = TOKEN_INVALID_NUMBER,
            },
        },
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test_digit), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct test_digit test = tests[i];
        struct scanner scanner = scanner_init(test.input);
        struct token got = scanner_next(&scanner);
        if (!token_equals(test.want, got)) {
            printf("test_scan_digit[idx=%d]: want != got\n", i);
            exit(1);
        }
    }

    printf("test_scan_digit: OK\n");
}


int main() {
    test_scan_digit();

    printf("\nAll tests done.\n");
}