#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t token_uint_t;

enum TokenType {
    STR_LIT_E,
    INT_LIT_E,
    FLOAT_LIT_E,
    SLASH_OP_E,
    COMMA_OP_E,
    BANG_OP_E,
    AT_OP_E,
    SHARP_OP_E,
    INSTRUCT_OP_E,
    COLON_OP_E,
};

enum TokenLineType {
    INSTRUCT,
    INCLUDE
};

struct Token {
    const char* string;
    token_uint_t length;
    token_uint_t column;
    enum TokenType type;
};

struct TokenLine {
    struct Token* tokens;
    token_uint_t count;
    token_uint_t capacity;
    token_uint_t indent;
    token_uint_t row;
    enum TokenLineType type;
};

struct TokenError {
    const char* message;
    struct {
        const char* string;
        token_uint_t length;
    } line;
    token_uint_t row;
    token_uint_t column;
};

struct TokenResult {
    struct TokenError error;
    struct TokenLine* lines;
    token_uint_t count;
    token_uint_t capacity;
};

int tokenize(const char* p_string, int (*p_handler)(bool p_success, const struct TokenResult p_result));
#endif //_TOKEN_H_
