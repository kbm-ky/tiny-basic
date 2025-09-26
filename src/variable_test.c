#include <tinybasic/variables.h>
#include <tinybasic/common.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int test_set_value() {
    struct variables variables = {};
    int rc = variables_init(&variables);
    if (rc != RC_SUCCESS) {
        printf("test_set_value: unable to init!\n");
        return 1;
    }

    int16_t in_value = 88;
    rc = variables_set(&variables, 'Z', in_value);
    if (rc != RC_SUCCESS) {
        printf("test_set_value: want %d, got %d\n", RC_SUCCESS, rc);
        return 1;
    }

    int16_t out_value = 0;
    rc = variables_get(&variables, 'Z', &out_value);
    if (rc != RC_SUCCESS) {
        printf("test_set_value: unable to get value!\n");
        return 1;
    }
    if (in_value != out_value) {
        printf("test_set_value: want %d, got %d\n", in_value, out_value);
        return 1;
    }

    variables_destroy(&variables);

    printf("test_set_value: OK\n");
    return 0;
}

int test_set_value_fail() {
    struct variables variables = {};
    int rc = variables_init(&variables);
    if (rc != RC_SUCCESS) {
        printf("test_set_value_fail: unable to init!\n");
        return 1;
    }

    rc = variables_set(&variables, 'a', 100);
    if (rc != RC_ERR_INVALID_VAR_INDEX) {
        printf("test_set_value_fail: expected index error!!\n");
        return 1;
    }

    rc = variables_set(&variables, 'Z'+1 , 100);
    if (rc != RC_ERR_INVALID_VAR_INDEX) {
        printf("test_set_value_fail: expected index error!!\n");
        return 1;
    }

    variables_destroy(&variables);

    printf("test_set_value_fail: OK\n");
    return 0;
}

int test_get_value_undefined() {
    struct variables variables = {};
    int rc = variables_init(&variables);
    if (rc != RC_SUCCESS) {
        printf("test_get_value_undefined: unable to init!\n");
        return 1;
    }

    int16_t value = 0;
    rc = variables_get(&variables, 'A', &value);
    if (rc != RC_ERR_VAR_NOT_DEFINED) {
        printf("test_get_value_undefined: expected undefined error!!\n");
        return 1;
    }

    variables_destroy(&variables);

    printf("test_get_value_undefined: OK\n");
    return 0;
}

int test_get_value_bad_index() {
    struct variables variables = {};
    int rc = variables_init(&variables);
    if (rc != RC_SUCCESS) {
        printf("test_get_value_bad_index: unable to init!\n");
        return 1;
    }

    int16_t value = 0;
    rc = variables_get(&variables, 'a', &value);
    if (rc != RC_ERR_INVALID_VAR_INDEX) {
        printf("test_get_value_bad_index: expected index error!!\n");
        return 1;
    }

    variables_destroy(&variables);

    printf("test_get_value_bad_index: OK\n");
    return 0;
}

int main() {
    printf("variable_test: ...\n\n");
    int count = 0;
    count += test_set_value();
    count += test_set_value_fail();
    count += test_get_value_undefined();
    count += test_get_value_bad_index();

    if (count > 0) {
        printf("\nvariable_test: %d test(s) failed!!!\n\n\n", count);
        return 1;
    }

    printf("\nvariable_test: All tests done.\n\n\n");
}