#include <tinybasic/parser.h>
#include <tinybasic/common.h>
#include <tinybasic/scanner.h>

#include <stdlib.h>
#include <stdbool.h>

// Checks if the parsers stream of tokens is empty/depleted
static bool _empty(struct parser *parser) {
    return parser->current.kind == TOKEN_EOF;
}

// Returns the current token
static struct token _peek(struct parser *parser) {
    return parser->current;
}

// Returns the current token and advances to the next
static struct token _next(struct parser *parser) {
    struct token current = parser->current;
    if (current.kind != TOKEN_EOF) {
        parser->current = scanner_next(parser->scanner);
    }
    return current;
}

// Checks if the current token matches the specified token kind
static bool _match(struct parser *parser, enum token_kind kind) {
    return parser->current.kind == kind;
}

// Constructs a parser
struct parser parser_init(struct scanner *scanner) {
    struct token current = scanner_next(scanner);
    return (struct parser){
        .scanner = scanner,
        .current = current,
    };
}

// Returns true if the next token is a number
bool parser_match_number(struct parser *parser) {
    return _match(parser, TOKEN_NUMBER);
}

// Parses a line beginning with a number
int parser_number_line(struct parser *parser, struct parser_num_line *result) {
    *result = (struct parser_num_line){}; //zero value

    // Number
    if (!_match(parser, TOKEN_NUMBER)) {
        return RC_EXPECTED_NUMBER;
    }

    struct token tok = _next(parser);
    const char *source = scanner_source(parser->scanner);
    int number = atoi(&source[tok.pos]);
    if (number < MIN_NUMBER || number > MAX_NUMBER) {
        return RC_INVALID_NUMBER;
    }   
    
    // Next token 
    tok = _next(parser);
    const char *line = &source[tok.pos];

    *result = (struct parser_num_line){
        .line = line,
        .number = number,
    };
    return RC_SUCCESS;
}



