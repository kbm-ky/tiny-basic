#include <tinybasic/interpreter.h>
#include <tinybasic/common.h>
#include <tinybasic/scanner.h>
#include "private.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct test {
    char *input;
    int16_t want;
};

int test_eval() {

    struct test tests[] = {
        { //0
            .input = "10",
            .want = 10,
        },
        { //1
            .input = "1024",
            .want = 1024,
        },
        { //2
            .input = "(1024)",
            .want = 1024,
        },
        { //3
            .input = "2 * 3",
            .want = 6,
        },
        { //4
            .input = "4 * 3 / 2",
            .want = 6,
        },
        { //5
            .input = "2 + 2",
            .want = 4,
        },
        { //6
            .input = "2 - 3",
            .want = -1,
        },
        { //7
            .input = "+2 - 3",
            .want = -1,
        },
        { //8
            .input = "-2 - 3",
            .want = -5,
        },
        { //9
            .input = "-2 * 3 - 1",
            .want = -7,
        },
        { //10
            .input = "-2 *( 3 - 1  )",
            .want = -4,
        },
    };

    enum { NUM_TESTS = sizeof(tests) / sizeof(struct test), };
    struct interpreter interpreter = interpreter_init(NULL);
    
    for (int i = 0; i < NUM_TESTS; i++) {
        int16_t got = 0;
        _reinit(&interpreter, tests[i].input);
        int rc = _eval(&interpreter, &got);
        //check return code
        if (tests[i].want != got) {
            printf("test_eval[idx=%d]: want != got!!!\n", i);
            printf("\twant %d\n", tests[i].want);
            printf("\tgot %d\n", got);
            return 1;
        }
    }

    printf("test_eval: OK\n");
    return 0;

}

int test_number() {

    char *input = "660";
    struct token token = {
        .kind = TOKEN_NUMBER,
        .pos = 0,
        .len = 3,
    };

    uint32_t result = 0;
    for (int i = 0; i < token.len; i++) {
        char *p = &input[token.pos+i];
        if (*p < '0' || *p > '9') {
            return RC_ERR_INVALID_NUMBER;
        }
        result *= 10;
        result += *p - '0';
        if (result > INT16_MAX) {
            return RC_ERR_INVALID_NUMBER;
        }
    }

    if (result != 660) {
        printf("test_number: fail!: %d\n", result);
    }

    printf("test_number: OK\n");
    return 0;

}

int main() {
    printf("interpreter_test: ...\n\n");
    int count = 0;
    count += test_eval();
    count += test_number();

    if (count > 0) {
        printf("\ninterpreter_test: %d test(s) failed!!!\n\n\n", count);
        return 1;
    }

    printf("\ninterpreter_test: All tests done.\n\n\n");
}