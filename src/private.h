#pragma once

#include <tinybasic/interpreter.h>
#include <stdint.h>

void _reinit(struct interpreter *interpreter, const char *source);
int _eval(struct interpreter *interpreter, int16_t *value); 