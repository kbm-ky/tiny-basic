#include <tinybasic/interpreter.h>
#include <tinybasic/common.h>
#include <tinybasic/scanner.h>
#include <tinybasic/editor.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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

static int _grouping(struct interpreter *interpreter, uint16_t *value) {
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

static int _primary(struct interpreter *interpreter, uint16_t *value) {
    if (_match(interpreter, TOKEN_NUMBER)) {
        return _number(interpreter, value);
    } else if (_match(interpreter, TOKEN_LPAREN)) {
        return _grouping(interpreter, value);
    }

    *value = 0;
    return RC_ERR_EXPECTED_EXPR;
}

static int _factor(struct interpreter *interpreter, uint16_t *value) {
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

static int _term(struct interpreter *interpreter, uint16_t *value) {

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

static int _print(struct interpreter *interpreter, struct editor *editor) {
    _next(interpreter); //eat PRINT

    if (_match(interpreter, TOKEN_STRING)) {
        struct token string = _next(interpreter);
        const char *start = &interpreter->source[string.pos+1]; //peel off leading "
        int len = string.len - 2; //account for ""
        editor_printf(editor, "%.*s", len, start);
    } else {
        uint16_t value = 0;
        int rc = _eval(interpreter, &value);
        if (rc != RC_SUCCESS) {
            return rc;
        }
        
        editor_printf(editor, "%d", (int)value);
    }

    //while commas...
    while (_match(interpreter, TOKEN_COMMA)) {
        _next(interpreter); //eat ,
        editor_printf(editor, ", ");

        if (_match(interpreter, TOKEN_STRING)) {
            struct token string = _next(interpreter);
            const char *start = &interpreter->source[string.pos+1]; //peel off leading "
            int len = string.len - 2; //account for ""
            editor_printf(editor, "%.*s", len, start);
        } else {
            uint16_t value = 0;
            int rc = _eval(interpreter, &value);
            if (rc != RC_SUCCESS) {
                return rc;
            }
        
            editor_printf(editor, "%d", (int)value);
        }
    }

    editor_println(editor, "");

    return RC_SUCCESS;
}



static int _statement(struct interpreter *interpreter, struct editor *editor) {
    if (_match(interpreter, TOKEN_PRINT)) {
        return _print(interpreter, editor);
    } else {
        return RC_ERR_EXPECTED_STATEMENT;
    }
}

static int _line(struct interpreter *interpreter, struct editor *editor) {
    return _statement(interpreter, editor);
}

static int interpret_line(struct interpreter *interpreter, struct editor *editor, char *line) {
    _reinit(interpreter, line);
    return _line(interpreter, editor);
}

int interpreter_loop(struct interpreter *interpreter, struct editor *editor) {
    while (1) {
        char *line = NULL;
        int rc = editor_prompt(editor, &line);
        if (rc == RC_ERR_INPUT_STRLEN_EXCEEDED) {
            editor_printf(editor, "Input string length exceeded.  Retry...\n\n");
            continue;
        } else if (rc != RC_SUCCESS) {
            editor_printf(editor, "Unrecoverable error\n\n");
            return 1;
        }

        rc = interpret_line(interpreter, editor, line);
        if (rc != RC_SUCCESS) {
            editor_printf(editor, "ERROR: %d\n", rc);
        }
    }
}