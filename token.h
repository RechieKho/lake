#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t token_uint_t;

enum token_type_e {
    STR_LIT_E,
    INT_LIT_E,
    FLOAT_LIT_E,
    SLASH_OP_E,
    BAR_OP_E,
    BANG_OP_E,
    AT_OP_E,
    SHARP_OP_E,
    INSTRUCT_OP_E,
    COLON_OP_E,
};

enum token_line_type_e {
    INSTRUCT,
    INCLUDE
};

struct token_s {
    const char* string;
    token_uint_t length;
    token_uint_t column;
    enum token_type_e type;
};

struct token_line_s {
    struct token_s* tokens;
    token_uint_t count;
    token_uint_t capacity;
    token_uint_t row;
    enum token_line_type_e type;
};

struct token_error_s {
    const char* message;
    struct {
        const char* string;
        token_uint_t length;
    } line;
    token_uint_t row;
    token_uint_t column;
};

struct token_result_s {
    struct token_error_s error;
    struct token_line_s* lines;
    token_uint_t count;
    token_uint_t capacity;
};

typedef int (*tokenize_handler_t)(bool p_success, const struct token_result_s p_result);

int tokenize(const char* p_string, tokenize_handler_t p_handler);
#endif //_TOKEN_H_
