#pragma once

#include <tinybasic/scanner.h>
#include <stdbool.h>

struct parser {
    struct scanner *scanner;
    struct token current;
};

// Result of parser_number_line
struct parser_num_line {
    const char *line;
    int number;
};

// Constructs a parser
struct parser parser_init(struct scanner *scanner);

// Returns true if the next token is a number
bool parser_match_number(struct parser *parser); 

// Parses a line beginning with a number
int parser_number_line(struct parser *parser, struct parser_num_line *result);