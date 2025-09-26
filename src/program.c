#include <tinybasic/program.h>
#include <tinybasic/common.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int program_init(struct program *program) {
    char *data = calloc(PROGRAM_ROWS*MAX_INPUT_SIZE, sizeof(char));
    if (data == NULL) {
        return RC_ERR_UNABLE_TO_ALLOC;
    }

    program->data = data;
    return RC_SUCCESS;
}

void program_destroy(struct program *program) {
    if (program->data != NULL) {
        free(program->data);
    }
}

int program_write_line(struct program *program, uint8_t line, const char *data) {
    size_t data_len = strlen(data);
    if (data_len > MAX_INPUT_STRLEN) {
        return RC_ERR_INPUT_TOO_BIG;
    }

    strncpy(&program->data[line*MAX_INPUT_SIZE], data, MAX_INPUT_SIZE);
    return RC_SUCCESS;
}

char *program_get_line(struct program *program, uint8_t line) {
    return &program->data[line*MAX_INPUT_SIZE];
}

void program_clear_all(struct program *program) {
    memset(program->data, 0, PROGRAM_SIZE);
}

void program_clear_line(struct program *program, uint8_t line) {
    memset(&program->data[line*MAX_INPUT_SIZE], 0, MAX_INPUT_SIZE);
}