#include <tinybasic/common.h>

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

enum { NUM_VARIABLES = 26, };

struct variables {
    int16_t *data;
    uint32_t defined;
};

int variables_init(struct variables *variables) {
    assert(variables);

    int16_t *data = calloc(NUM_VARIABLES, sizeof(int16_t));
    if (data == NULL) {
        return RC_ERR_UNABLE_TO_ALLOC;
    }

    *variables = (struct variables){
        .data = data,
        .defined = 0,
    };
    return RC_SUCCESS;
}

void variables_destroy(struct variables *variables) {
    assert(variables);

    if (variables->data != NULL) {
        free(variables->data);
    }
}

static int _name_to_index(char name) {
    if (name < 'A' || name > 'Z') {
        return -1;
    }
    int index = (name - 'A');
    assert(index >= 0 && index < 26);

    return index;
}

int variables_set(struct variables *variables, char name, int16_t value) {
    assert(variables);

    int index = _name_to_index(name);
    if (index < 0) {
        return RC_ERR_INVALID_VAR_INDEX;
    }

    variables->data[index] = value;
    variables->defined |= (1 << index);

    return RC_SUCCESS;
}

int variables_get(struct variables *variables, char name, int16_t *value) {
    assert(variables);
    assert(value);

    int index = _name_to_index(name);
    if (index < 0) {
        *value = 0;
        return RC_ERR_INVALID_VAR_INDEX;
    }

    int32_t defined = variables->defined & (1 << index);
    if (!defined) {
        *value = 0;
        return RC_ERR_VAR_NOT_DEFINED;
    }

    *value = variables->data[index];

    return RC_SUCCESS;
}