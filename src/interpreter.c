#include <tinybasic/interpreter.h>
#include <tinybasic/common.h>
#include <tinybasic/scanner.h>
#include <tinybasic/editor.h>
#include <tinybasic/variables.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// Prototypes
int _eval(struct interpreter *interpreter, int16_t *value);
static int _statement(struct interpreter *interpreter, struct editor *editor);

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

static bool _match_relop(struct interpreter *interpreter) {
    struct token tok = _peek(interpreter);
    switch (tok.kind) {
        case TOKEN_EQUAL:
        case TOKEN_NEQUAL:
        case TOKEN_LT:
        case TOKEN_LTE:
        case TOKEN_GT:
        case TOKEN_GTE:
            return true;
        
        default:
            return false;
    }
}

// Re-initialize the scanner and current token with the new source string
void _reinit(struct interpreter *interpreter, const char *source) {
    interpreter->source = source;
    interpreter->scanner = scanner_init(source);
    interpreter->current = scanner_next(&interpreter->scanner);
}

// Constructs a interpreter
struct interpreter interpreter_init(struct variables *variables) {
    return (struct interpreter){
        .source = "",
        .scanner = scanner_init(""),
        .current = TOKEN_EOF,
        .variables = variables,
        .mode = INTERPRETER_MODE_DIRECT,
    };
}

void interpreter_destroy(struct interpreter *interpreter) {

}

static int _number(struct interpreter *interpreter, int16_t *value) {
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

    *value = (int16_t)result;
    return RC_SUCCESS;
}

static int _grouping(struct interpreter *interpreter, int16_t *value) {
    _next(interpreter); //eat (
        
    // eval the expression inside the parens
    int16_t tmp = 0;
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
    _next(interpreter);
        
    *value = tmp;
    return RC_SUCCESS;
}

static int _variable(struct interpreter *interpreter, int16_t *value) {
    struct token token = _next(interpreter);
    int16_t tmp = 0;
    int rc = variables_get(interpreter->variables, interpreter->source[token.pos], &tmp);
    if (rc != RC_SUCCESS) {
        return rc;
    }

    *value = tmp;
    return RC_SUCCESS;
}

static int _primary(struct interpreter *interpreter, int16_t *value) {
    if (_match(interpreter, TOKEN_NUMBER)) {
        return _number(interpreter, value);
    } else if (_match(interpreter, TOKEN_LPAREN)) {
        return _grouping(interpreter, value);
    } else if (_match(interpreter, TOKEN_VARIABLE)) {
        return _variable(interpreter, value);
    }

    *value = 0;
    return RC_ERR_EXPECTED_EXPR;
}

static int _factor(struct interpreter *interpreter, int16_t *value) {
    int16_t factor = 0;
    int rc = _primary(interpreter, &factor);
    if (rc != RC_SUCCESS) {
        *value = 0;
        return rc;
    }

    while (_match(interpreter, TOKEN_STAR) || _match(interpreter, TOKEN_SLASH)) {
        struct token op = _next(interpreter);

        int16_t tmp = 0;
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

static int _term(struct interpreter *interpreter, int16_t *value) {

    // leading +/- sign?
    int16_t sign = 1;
    if (_match(interpreter, TOKEN_PLUS)) {
        //just eat
        _next(interpreter);
    } else if(_match(interpreter, TOKEN_MINUS)) {
        //eat
        _next(interpreter);
        sign = -1;
    }

    int16_t term = 0;
    int rc = _factor(interpreter, &term);
    if (rc != RC_SUCCESS) {
        *value = 0;
        return rc;
    }
    term *= sign;

    while (_match(interpreter, TOKEN_PLUS) || _match(interpreter, TOKEN_MINUS)) {
        struct token op = _next(interpreter);

        int16_t tmp = 0;
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


int _eval(struct interpreter *interpreter, int16_t *value) {
    
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
        int16_t value = 0;
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
            int16_t value = 0;
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

static int _let(struct interpreter *interpreter, struct editor *editor) {
    _next(interpreter); //eat LET

    //var
    if (!_match(interpreter, TOKEN_VARIABLE)) {
        return RC_ERR_EXPECTED_VARIABLE;
    }
    struct token var = _next(interpreter);

    //=
    if (!_match(interpreter, TOKEN_EQUAL)) {
        return RC_ERR_EXPECTED_EQUAL;
    }
    _next(interpreter); //eat =

    //expression
    int16_t value = 0;
    int rc = _eval(interpreter, &value);
    if (rc != RC_SUCCESS) {
        return rc;
    }

    rc = variables_set(interpreter->variables, interpreter->source[var.pos], value);
    if (rc != RC_SUCCESS) {
        return rc;
    }

    //do the thing
    return RC_SUCCESS;
}

static int _if(struct interpreter *interpreter, struct editor *editor) {
    _next(interpreter); //eat IF

    int16_t left_expr = 0;
    int rc = _eval(interpreter, &left_expr);
    if (rc != RC_SUCCESS) {
        return rc;
    }

    if (!_match_relop(interpreter)) {
        return RC_ERR_EXPECTED_RELOP;
    }
    struct token relop = _next(interpreter);

    int16_t right_expr = 0;
    rc = _eval(interpreter, &right_expr);
    if (rc != RC_SUCCESS) {
        return rc;
    }

    bool condition = false;
    switch (relop.kind) {
        case TOKEN_EQUAL:
            condition = left_expr == right_expr;
            break;

        case TOKEN_NEQUAL:
            condition = left_expr != right_expr;
            break;

        case TOKEN_LT:
            condition = left_expr < right_expr;
            break;

        case TOKEN_LTE:
            condition = left_expr <= right_expr;
            break;

        case TOKEN_GT:
            condition = left_expr > right_expr;
            break;

        case TOKEN_GTE:
            condition = left_expr >= right_expr;
            break;
    } 

    //eat THEN
    if (!_match(interpreter, TOKEN_THEN)) {
        return RC_ERR_EXPECTED_THEN;
    }
    _next(interpreter);

    if (condition) {
        return _statement(interpreter, editor);
    } else {
        //eat all tokens until end of line....even if syntax is wrong
        while (!_match(interpreter, TOKEN_NEWLINE)) {
            _next(interpreter);
        }
        return RC_SUCCESS;
    }
}

static int _statement(struct interpreter *interpreter, struct editor *editor) {
    if (_match(interpreter, TOKEN_PRINT)) {
        return _print(interpreter, editor);
    } else if (_match(interpreter, TOKEN_LET)) {
        return  _let(interpreter, editor);
    } else if (_match(interpreter, TOKEN_IF)) {
        return _if(interpreter, editor);
    } else {
        return RC_ERR_EXPECTED_STATEMENT;
    }
}

static int _line(struct interpreter *interpreter, struct editor *editor) {
    if (_match(interpreter, TOKEN_NEWLINE) || _match(interpreter, TOKEN_EOF)) {
        return RC_SUCCESS;
    }
    
    int rc = _statement(interpreter, editor);
    if (rc != RC_SUCCESS) {
        return rc;
    }

    //expect CRLF
    if (!_match(interpreter, TOKEN_NEWLINE)) {
        return RC_ERR_EXPECTED_NEWLINE;
    }
    _next(interpreter);

    return RC_SUCCESS;
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