#include <tinybasic/scanner.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool token_equals(struct token a, struct token b) {
    return  (a.pos == b.pos) &&
            (a.len == b.len) &&   
            (a.kind == b.kind);
}

void token_print(struct token a) {
    printf("token{pos:%d, len:%d, kind:%d}", a.pos, a.len, a.kind);
}

struct test {
    char *input;
    struct token want;
};

void test_scan_digit() {

    struct test tests[] = {
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

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct test test = tests[i];
        struct scanner scanner = scanner_init(test.input);
        struct token got = scanner_next(&scanner);
        if (!token_equals(test.want, got)) {
            printf("test_scan_digit[idx=%d]: want != got\n", i);
            printf("want ");
            token_print(test.want);
            printf("\ngot ");
            token_print(got);
            printf("\n");
            exit(1);
        }
    }

    printf("test_scan_digit: OK\n");
}

void test_scan_ident() {

    struct test tests[] = {
        { //0
            .input = " PRINT ",
            .want = {
                .pos = 1,
                .len = 5,
                .kind = TOKEN_PRINT,
            },
        },
        { //1
            .input = "IF",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_IF,
            },
        },
        { //2
            .input = "THEN",
            .want = {
                .pos = 0,
                .len = 4,
                .kind = TOKEN_THEN,
            },
        },
        { //3
            .input = "GOTO",
            .want = {
                .pos = 0,
                .len = 4,
                .kind = TOKEN_GOTO,
            },
        },
        { //4
            .input = "INPUT",
            .want = {
                .pos = 0,
                .len = 5,
                .kind = TOKEN_INPUT,
            },
        },
        { //5
            .input = "LET",
            .want = {
                .pos = 0,
                .len = 3,
                .kind = TOKEN_LET,
            },
        },
        { //6
            .input = "GOSUB",
            .want = {
                .pos = 0,
                .len = 5,
                .kind = TOKEN_GOSUB,
            },
        },
        { //7
            .input = "RETURN",
            .want = {
                .pos = 0,
                .len = 6,
                .kind = TOKEN_RETURN,
            },
        },
        { //8
            .input = "CLEAR",
            .want = {
                .pos = 0,
                .len = 5,
                .kind = TOKEN_CLEAR,
            },
        },
        { //9
            .input = "LIST",
            .want = {
                .pos = 0,
                .len = 4,
                .kind = TOKEN_LIST,
            },
        },
        { //10
            .input = "RUN",
            .want = {
                .pos = 0,
                .len = 3,
                .kind = TOKEN_RUN,
            },
        },
        { //11
            .input = "END",
            .want = {
                .pos = 0,
                .len = 3,
                .kind = TOKEN_END,
            },
        },
        { //12
            .input = "A",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_VARIABLE,
            },
        },
        { //13
            .input = "Z",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_VARIABLE,
            },
        },
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct test test = tests[i];
        struct scanner scanner = scanner_init(test.input);
        struct token got = scanner_next(&scanner);
        if (!token_equals(test.want, got)) {
            printf("test_scan_ident[idx=%d]: want != got\n", i);
            printf("want ");
            token_print(test.want);
            printf("\ngot ");
            token_print(got);
            printf("\n");
            exit(1);
        }
    }

    printf("test_scan_ident: OK\n");
}

void test_scan_relop() {

    struct test tests[] = {
        { //0
            .input = "=",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_EQUAL,
            },
        },
        { //1
            .input = "<",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_LT,
            },
        },
        { //2
            .input = ">",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_GT,
            },
        },
        { //3
            .input = ">=",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_GTE,
            },
        },
        { //4
            .input = "><",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_NEQUAL,
            },
        },
        { //5
            .input = "<=",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_LTE,
            },
        },
        { //6
            .input = "<>",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_NEQUAL,
            },
        },
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct test test = tests[i];
        struct scanner scanner = scanner_init(test.input);
        struct token got = scanner_next(&scanner);
        if (!token_equals(test.want, got)) {
            printf("test_scan_relop[idx=%d]: want != got\n", i);
            printf("want ");
            token_print(test.want);
            printf("\ngot ");
            token_print(got);
            printf("\n");
            exit(1);
        }
    }

    printf("test_scan_relop: OK\n");
}

void test_scan_small_stuff() {

    struct test tests[] = {
        { //0
            .input = ",",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_COMMA,
            },
        },
        { //1
            .input = "\r",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_NEWLINE,
            },
        },
        { //2
            .input = "\r\n",
            .want = {
                .pos = 0,
                .len = 2,
                .kind = TOKEN_NEWLINE,
            },
        },
        { //3
            .input = "\n",
            .want = {
                .pos = 0,
                .len = 1,
                .kind = TOKEN_NEWLINE,
            },
        },
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test), };
    
    for (int i = 0; i < NUM_TESTS; i++) {
        struct test test = tests[i];
        struct scanner scanner = scanner_init(test.input);
        struct token got = scanner_next(&scanner);
        if (!token_equals(test.want, got)) {
            printf("test_scan_small_stuff[idx=%d]: want != got\n", i);
            printf("want ");
            token_print(test.want);
            printf("\ngot ");
            token_print(got);
            printf("\n");
            exit(1);
        }
    }

    printf("test_scan_small_stuff: OK\n");
}

int main() {
    test_scan_digit();
    test_scan_ident();
    test_scan_relop();
    test_scan_small_stuff();

    printf("\nAll tests done.\n");
}