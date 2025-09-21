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


int main() {
    test_scan_digit();

    printf("\nAll tests done.\n");
}