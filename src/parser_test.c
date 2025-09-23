#include <tinybasic/common.h>
#include <tinybasic/parser.h>
#include <tinybasic/scanner.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

int test_parse_statement() {
    printf("test_parse_statement: NG\n");
    return 1;
}

int main() {
    printf("parser_test: ...\n\n");
    int count = 0;
    count += test_parse_match_number();
    count += test_parse_number_line();
    count += test_parse_number_line_fail();
    count += test_parse_statement();

    if (count > 0) {
        printf("\nparser_test: %d test(s) failed!!!\n\n\n", count);
        return 1;
    }

    printf("\nparser_test: All tests done.\n\n\n");
}