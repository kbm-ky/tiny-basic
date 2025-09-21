#include <tinybasic/parser.h>
#include <tinybasic/scanner.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>



struct parser {
    struct scanner *scanner;
    struct token current;
};

static bool _empty(struct parser *parser) {
    return parser->current.kind == TOKEN_EOF;
}

static struct token _peek(struct parser *parser) {
    return parser->current;
}

static struct token _next(struct parser *parser) {
    struct token current = parser->current;
    if (current.kind != TOKEN_EOF) {
        parser->current = scanner_next(parser->scanner);
    }
    return current;
}

static bool _match(struct parser *parser, enum token_kind kind) {
    return parser->current.kind == kind;
}

struct parser parser_init(struct scanner *scanner) {
    struct token current = scanner_next(scanner);
    return (struct parser){
        .scanner = scanner,
        .current = current,
    };
}

bool parser_match_number(struct parser *parser) {
    return _match(parser, TOKEN_NUMBER);
}


struct test_match_number {
    char *input;
    bool want;
};

int test_parse_match_number() {

    struct test_match_number tests[] = {
        { //0
            .input = "10 GOTO 10",
            .want = true,
        },
        { //1
            .input = "GOTO 10",
            .want = false,
        },
    
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test_match_number), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct scanner scanner = scanner_init(tests[i].input);
        struct parser parser = parser_init(&scanner);
        bool got = parser_match_number(&parser);
        if (tests[i].want != got) {
            printf("test_parse_match_number[idx=%d]: want != got!!!\n", i);
            printf("\twant %d\n", tests[i].want);
            printf("\tgot %d\n", got);
            return 1;
        }
    }

    printf("test_parse_match_number: OK\n");
    return 0;

}

struct parser_num_line {
    const char *line;
    int number;
};

const char *scanner_source(struct scanner *scanner) {
    return scanner->source;
}

enum {
    RC_SUCCESS = 0,
    RC_INVALID_NUMBER,
    RC_EXPECTED_NUMBER,

    MIN_NUMBER = 1,
    MAX_NUMBER = 256,
};

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

struct test_number_line {
    char *input;
    int want_rc;
    struct parser_num_line want;
};

int test_parse_number_line() {

    struct test_number_line tests[] = {
        { //0
            .input = "10 GOTO 10",
            .want_rc = RC_SUCCESS,
            .want = {
                .line = "GOTO 10",
                .number = 10,
            },
        },
        { //1
            .input = "8 LET A = 100",
            .want_rc = RC_SUCCESS,
            .want = {
                .line = "LET A = 100",
                .number = 8,
            },
        },
        { //1
            .input = "8 ",
            .want_rc = RC_SUCCESS,
            .want = {
                .line = "",
                .number = 8,
            },
        },
    
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test_number_line), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct scanner scanner = scanner_init(tests[i].input);
        struct parser parser = parser_init(&scanner);
        struct parser_num_line number_line = {};
        int got = parser_number_line(&parser, &number_line);
        //check return code
        if (tests[i].want_rc != got) {
            printf("test_parse_number_line[idx=%d]: want_rc != got!!!\n", i);
            printf("\twant_rc %d\n", tests[i].want_rc);
            printf("\tgot %d\n", got);
            return 1;
        }

        //check returned struct
        if (strcmp(tests[i].want.line, number_line.line) != 0) {
            printf("test_parse_number_line[idx=%d]: want result != got result !!!\n", i);
            printf("\twant '%s', got '%s'\n", tests[i].want.line, number_line.line);
            return 1;
        }

        if (tests[i].want.number != number_line.number) {
            printf("test_parse_number_line[idx=%d]: want result != got result!!!\n", i);
            printf("\twant '%d', got '%d'\n", tests[i].want.number, number_line.number);
            return 1;
        }
    }

    printf("test_parse_number_line: OK\n");
    return 0;

}

int test_parse_number_line_fail() {

    struct test_number_line tests[] = {
        { //0
            .input = "0 GOTO 10",
            .want_rc = RC_INVALID_NUMBER,
        },
        { //1
            .input = "257 GOTO 10",
            .want_rc = RC_INVALID_NUMBER,
        },
        { //2
            .input = "GOTO 10",
            .want_rc = RC_EXPECTED_NUMBER,
        },
        { //3
            .input = "",
            .want_rc = RC_EXPECTED_NUMBER,
        },
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test_number_line), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct scanner scanner = scanner_init(tests[i].input);
        struct parser parser = parser_init(&scanner);
        struct parser_num_line number_line = {};
        int got = parser_number_line(&parser, &number_line);
        //check return code
        if (tests[i].want_rc != got) {
            printf("test_parse_number_line_fail[idx=%d]: want_rc != got!!!\n", i);
            printf("\twant_rc %d\n", tests[i].want_rc);
            printf("\tgot %d\n", got);
            return 1;
        }
    }

    printf("test_parse_number_line_fail: OK\n");
    return 0;

}

int main() {
    printf("parser_test: ...\n\n");
    int count = 0;
    count += test_parse_match_number();
    count += test_parse_number_line();
    count += test_parse_number_line_fail();

    if (count > 0) {
        printf("\nparser_test: %d test(s) failed!!!\n\n\n", count);
        return 1;
    }

    printf("\nparser_test: All tests done.\n\n\n");
}