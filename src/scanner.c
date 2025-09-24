#include <tinybasic/scanner.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Checks if the scanner's input is empty/depleted
static bool _empty(struct scanner *scanner) {
    return scanner->pos >= strlen(scanner->source);
}

// Sets a mark at the current scanner character position
static int _mark(struct scanner *scanner) {
    return (scanner->mark = scanner->pos);
}

// Peeks at the current character
static char _peek(struct scanner *scanner) {
    return scanner->source[scanner->pos];
}

// Returns the current character and advances the scanner position to the next
static char _next(struct scanner *scanner) {
    return scanner->source[scanner->pos++];
}

// Returns true if ch is a 'whitespace' character, excluding CR LF
static bool _isspace(char ch) {
    return  (ch == ' ') || (ch == '\f') || (ch == '\t') || (ch == '\v');
}

// Skips over whitespace
static void _skipwhitespace(struct scanner *scanner) {
    while (!_empty(scanner)) {
        char ch = _peek(scanner);
        if (!_isspace(ch)) return;
        _next(scanner);
    }
}

// Constructs the specified token type from the current scanner state
static struct token _make_token(struct scanner *scanner, enum token_kind kind) {
    int len = scanner->pos - scanner->mark;
    return (struct token) {
        .pos = scanner->mark,
        .len = len,
        .kind = kind,
    };
}

// Scans a number: (0..9)+
static struct token _number(struct scanner *scanner) {
    _next(scanner); //eat digit from scan_next

    while (!_empty(scanner)) {
        char ch = _peek(scanner);
        if (!isdigit(ch)) break;
        _next(scanner);
    }

    //number cannot end with a letter, e.g. 100ABC
    if (!_empty(scanner) && isalpha(_peek(scanner))) {
        _next(scanner); //eat offending letter
        return _make_token(scanner, TOKEN_INVALID_NUMBER);
    }

    return _make_token(scanner, TOKEN_NUMBER);
}

// Utility struct for scanning language keywords
struct keyword_entry {
    const char *name;
    enum token_kind kind;
};

// A table of keyword entries
static const struct keyword_entry keywords[] = {
    { "PRINT", TOKEN_PRINT },
    { "IF", TOKEN_IF },
    { "THEN", TOKEN_THEN },
    { "GOTO", TOKEN_GOTO },
    { "INPUT", TOKEN_INPUT },
    { "LET", TOKEN_LET },
    { "GOSUB", TOKEN_GOSUB },
    { "RETURN", TOKEN_RETURN },
    { "CLEAR", TOKEN_CLEAR },
    { "LIST", TOKEN_LIST },
    { "RUN", TOKEN_RUN },
    { "END", TOKEN_END },
};

enum { NUM_KEYWORDS = sizeof(keywords) / sizeof(struct keyword_entry), };

// Checks if the in-process token is a keyword, otherwise returns
// an undefined token
static struct token _lookup_keyword(struct scanner *scanner) {
    int len = scanner->pos - scanner->mark;
    const char *tok = &scanner->source[scanner->mark];

    for (int i = 0; i < NUM_KEYWORDS; i++) {
        struct keyword_entry keyword_entry = keywords[i];
        if (len != strlen(keyword_entry.name)) continue;
        
        //compare bytes
        if (memcmp(keyword_entry.name, tok, len) == 0) {
            //A match
            return _make_token(scanner, keyword_entry.kind);
        }
    }

    return _make_token(scanner, TOKEN_UNDEFINED);
}

// Scans an 'ident' which could be a keyword or variable name (or undefined)
static struct token _ident(struct scanner *scanner) {
    _next(scanner);  //eat letter from scan_next

    while (!_empty(scanner)) {
        if (!isupper(_peek(scanner))) {
            break;
        }
        _next(scanner);
    }
    
    //ident cannot end with number
    if (!_empty(scanner) && isdigit(_peek(scanner))) {
        _next(scanner);  //eat offending digit
        return _make_token(scanner, TOKEN_INVALID_IDENTIFIER);
    }

    //Figure out what this thing is
    //If it is 1-char, then it's a variable, isupper ensures that it is A-Z
    int len = scanner->pos - scanner->mark;
    if (len == 1) {
        return _make_token(scanner, TOKEN_VARIABLE);
    }

    //It could be a keyword, delegate that
    return _lookup_keyword(scanner);
}

// Returns true if ch is a valid string character
static bool _isstring(char ch) {
    return  (ch == ' ') || (ch == '!') ||
            (ch >= '#' && ch <= '~');
}

// Scans a string literal, including its double-quotes ""
static struct token _string(struct scanner *scanner) {
    _next(scanner); //eat "

    while (!_empty(scanner)) {
        if (!_isstring(_peek(scanner))) break;
        _next(scanner);
    }

    //Did we hit a " ?
    if (!_empty(scanner) && _peek(scanner) == '\"') {
        _next(scanner);
        return _make_token(scanner, TOKEN_STRING);
    }

    // invalid/unbalanced string
    return _make_token(scanner, TOKEN_INVALID_STRING);
}

// Constructs a scanner for the given source string
struct scanner scanner_init(const char *source) {
    return (struct scanner){
        .source = source,
        .pos = 0,
        .mark = 0,
    };
}

// Returns the scanner's source string
const char *scanner_source(struct scanner *scanner) {
    return scanner->source;
}

// Returns the current token and advances to the next
struct token scanner_next(struct scanner *scanner) {
    _skipwhitespace(scanner);
    _mark(scanner);

    if (_empty(scanner)) {
        return _make_token(scanner, TOKEN_EOF);
    };

    char ch = _peek(scanner);

    if (isdigit(ch)) {
        return _number(scanner);
    }  else if (isupper(ch)) {
        return _ident(scanner);
    } else if (ch == '\"') { 
        return _string(scanner);  
    } else if (ch == '=') {
        _next(scanner);
        return _make_token(scanner, TOKEN_EQUAL);
    } else if (ch == '<') { 
        _next(scanner);
        if (!_empty(scanner)) {
            char ch = _peek(scanner);
            if (ch == '=') {
                _next(scanner);
                return _make_token(scanner, TOKEN_LTE);
            } else if (ch == '>') {
                _next(scanner);
                return _make_token(scanner, TOKEN_NEQUAL);
            }
        }
        return _make_token(scanner, TOKEN_LT);
    } else if (ch == '>') {
        _next(scanner);
        if (!_empty(scanner)) {
            char ch = _peek(scanner);
            if (ch == '=') {
                _next(scanner);
                return _make_token(scanner, TOKEN_GTE);
            } else if (ch == '<') {
                _next(scanner);
                return _make_token(scanner, TOKEN_NEQUAL);
            }
        }
        return _make_token(scanner, TOKEN_GT);
    } else if (ch == ',') { 
        _next(scanner);
        return _make_token(scanner, TOKEN_COMMA);
    } else if (ch == '\r') {
        _next(scanner);
        if (!_empty(scanner) && _peek(scanner) == '\n') {
            _next(scanner);
        }  
        return _make_token(scanner, TOKEN_NEWLINE);
    } else if (ch == '\n') {
        _next(scanner);
        return _make_token(scanner, TOKEN_NEWLINE); 
    } else if (ch == '(') {
        _next(scanner);
        return _make_token(scanner, TOKEN_LPAREN);
    } else if (ch == ')') {
        _next(scanner);
        return _make_token(scanner, TOKEN_RPAREN);
    } else if (ch == '+') {
        _next(scanner);
        return _make_token(scanner, TOKEN_PLUS);
    } else if (ch == '-') {
        _next(scanner);
        return _make_token(scanner, TOKEN_MINUS);
    } else if (ch == '*') {
        _next(scanner);
        return _make_token(scanner, TOKEN_STAR);
    } else if (ch == '/') {
        _next(scanner);
        return _make_token(scanner, TOKEN_SLASH);
    } else {
        _next(scanner);
        return _make_token(scanner, TOKEN_UNDEFINED);
    }
}