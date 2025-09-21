#include <tinybasic/scanner.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


static bool _empty(struct scanner *scanner) {
    return scanner->pos >= strlen(scanner->source);
}

static int _mark(struct scanner *scanner) {
    return (scanner->mark = scanner->pos);
}

static char _peek(struct scanner *scanner) {
    return scanner->source[scanner->pos];
}

static char _next(struct scanner *scanner) {
    return scanner->source[scanner->pos++];
}

static void _skipwhitespace(struct scanner *scanner) {
    while (!_empty(scanner)) {
        char ch = _peek(scanner);
        if (!isspace(ch)) return;
        _next(scanner);
    }
}

static struct token _make_token(struct scanner *scanner, enum token_kind kind) {
    int len = scanner->pos - scanner->mark;
    return (struct token) {
        .pos = scanner->mark,
        .len = len,
        .kind = kind,
    };
}

static struct token _number(struct scanner *scanner) {
    _next(scanner); //eat digit from scan_next

    while (!_empty(scanner)) {
        char ch = _peek(scanner);
        if (!isdigit(ch)) break;
        _next(scanner);
    }

    //number cannot end with a letter, e.g. 100ABC
    if (!_empty(scanner) && isalpha(_peek(scanner))) {
        _next(scanner); //eat offending letter
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