#include <tinybasic/interpreter.h>
#include <tinybasic/common.h>
#include <tinybasic/scanner.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Prototypes
int _eval(struct interpreter *interpreter, uint16_t *value);

// Checks if the stream of tokens is empty/depleted
static bool _empty(struct interpreter *interpreter) {
    return interpreter->current.kind == TOKEN_EOF;
}

// Returns the current token
static struct token _peek(struct interpreter *interpreter) {
    return interpreter->current;
}

// Returns the current token and advances to the next
static struct token _next(struct interpreter *interpreter) {
    struct token current = interpreter->current;
    if (current.kind != TOKEN_EOF) {
        interpreter->current = scanner_next(&interpreter->scanner);
    }
    return current;
}

// Checks if the current token matches the specified token kind
static bool _match(struct interpreter *interpreter, enum token_kind kind) {
    return interpreter->current.kind == kind;
}

// Re-initialize the scanner and current token with the new source string
void _reinit(struct interpreter *interpreter, const char *source) {
    interpreter->source = source;
    interpreter->scanner = scanner_init(source);
    interpreter->current = scanner_next(&interpreter->scanner);
}

// Constructs a interpreter
struct interpreter interpreter_init() {
    return (struct interpreter){
        .source = "",
        .scanner = scanner_init(""),
        .current = TOKEN_EOF,
    };
}

static int _number(struct interpreter *interpreter, uint16_t *value) {
    if (!_match(interpreter, TOKEN_NUMBER)) {
        *value = 0;
        return RC_ERR_EXPECTED_NUMBER;
    }

    struct token token = _next(interpreter);
    uint32_t result = 0;
    const char *source = interpreter->source;
    for (int i = 0; i < token.len; i++) {
        const char *p = &source[token.pos+i];
        if (*p < '0' || *p > '9') {
            return RC_ERR_INVALID_NUMBER;
        }
        result *= 10;
        result += *p - '0';
        if (result > INT16_MAX) {
            return RC_ERR_INVALID_NUMBER;
        }
    }

    *value = (uint16_t)result;
    return RC_SUCCESS;
}

int _grouping(struct interpreter *interpreter, uint16_t *value) {
    _next(interpreter); //eat (
        
    // eval the expression inside the parens
    uint16_t tmp = 0;
    int rc = _eval(interpreter, &tmp);
    if (rc != RC_SUCCESS) {
        *value = 0;
        return rc;
    } 

    // eat the )
    if (!_match(interpreter, TOKEN_RPAREN)) {
        *value = 0;
        return RC_ERR_EXPECTED_RPAREN;
    }
        
    *value = tmp;
    return RC_SUCCESS;
}

int _primary(struct interpreter *interpreter, uint16_t *value) {
    if (_match(interpreter, TOKEN_NUMBER)) {
        return _number(interpreter, value);
    } else if (_match(interpreter, TOKEN_LPAREN)) {
        return _grouping(interpreter, value);
    }

    *value = 0;
    return RC_ERR_EXPECTED_EXPR;
}

int _factor(struct interpreter *interpreter, uint16_t *value) {
    uint16_t factor = 0;
    int rc = _primary(interpreter, &factor);
    if (rc != RC_SUCCESS) {
        *value = 0;
        return rc;
    }

    while (_match(interpreter, TOKEN_STAR) || _match(interpreter, TOKEN_SLASH)) {
        struct token op = _next(interpreter);

        uint16_t tmp = 0;
        rc = _primary(interpreter, &tmp);
        if (rc != RC_SUCCESS) {
            *value = 0;
            return rc;
        }

        if (op.kind == TOKEN_STAR) {
            factor *= tmp;
        } else {
            factor /= tmp;
        }
    }

    *value = factor;
    return RC_SUCCESS;
}

int _term(struct interpreter *interpreter, uint16_t *value) {

    // leading +/- sign?
    uint16_t sign = 1;
    if (_match(interpreter, TOKEN_PLUS)) {
        //just eat
        _next(interpreter);
    } else if(_match(interpreter, TOKEN_MINUS)) {
        //eat
        _next(interpreter);
        sign = -1;
    }

    uint16_t term = 0;
    int rc = _factor(interpreter, &term);
    if (rc != RC_SUCCESS) {
        *value = 0;
        return rc;
    }
    term *= sign;

    while (_match(interpreter, TOKEN_PLUS) || _match(interpreter, TOKEN_MINUS)) {
        struct token op = _next(interpreter);

        uint16_t tmp = 0;
        rc = _factor(interpreter, &tmp);
        if (rc != RC_SUCCESS) {
            *value = 0;
            return rc;
        }

        if (op.kind == TOKEN_PLUS) {
            term += tmp;
        } else {
            term -= tmp;
        }
    }

    *value = term;
    return RC_SUCCESS;
}


int _eval(struct interpreter *interpreter, uint16_t *value) {
    
    return _term(interpreter, value);
}

int _print(struct interpreter *interpreter) {
    _next(interpreter); //eat PRINT

    //TODO
    return RC_ERR_PLACEHOLDER;
}

// Interprets a statement 
int interpret(struct interpreter *interpreter, const char *source) {
    _reinit(interpreter, source);

    struct token token = _peek(interpreter);
    
    if (token.kind == TOKEN_EOF) {
        return RC_SUCCESS;
    }

    if (token.kind == TOKEN_PRINT) {
        return _print(interpreter);
    }

    return RC_ERR_PLACEHOLDER;
}
