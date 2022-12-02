#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "macros.h"

#define INIT_TOKEN_CAPACITY 20
#define INIT_LINE_CAPACITY 20

#define IS_TOKEN_STRLIT(mp_token) (mp_token->length >= 2 && *mp_token->string == '\'')
#define IS_TOKEN_STR(mp_token, mp_str) ( \
        (strlen(mp_str) == mp_token->length) && \
        !strncmp(mp_str, mp_token->string, mp_token->length))
#define RAISE_ERROR(mp_error, mp_message, mp_label) do { \
    mp_error.message = mp_message; \
    goto mp_label; \
} while(false); 

#define APPEND_TOKEN(mp_line, mp_token, mp_error, mp_label) do { \
    define_token(&mp_token); \
    if(!append_token(&mp_line, mp_token)) RAISE_ERROR(mp_error, error_strings.memory_error, mp_label); \
} while(false);
#define PARSE_LINE(mp_string, mp_length, mp_row, mp_result, mp_label) do { \
        struct TokenLine line = {0}; \
        if(!parse_line(mp_string, mp_length, &line, &mp_result.error)) goto mp_label; \
        if(line.capacity) { \
            if(define_line(&line)) { \
                line.row = mp_row; \
                if(!append_line(&mp_result, line)) { \
                    if(line.capacity) free(line.tokens); \
                    RAISE_ERROR(mp_result.error, error_strings.memory_error, mp_label); \
                } \
            } else if(line.capacity) free(line.tokens); \
        } \
    } while(false);

static const struct TokenError ok = {0};
static const struct {
    const char* mixed_indentation;
    const char* unterminated_strlit;
    const char* memory_error;
} error_strings = {
    "Mixed indentation is forbidden.",
    "Unterminated string literal.",
    "Fail to allocated memory.",
};

static const char* standalone_chars = "(){}[]@,";
static const char* operator_chars = "!:/#";
static const char* strlit_chars = "\"'";
static const char* indent_chars = " \t";

static bool append_token(struct TokenLine* p_line, const struct Token p_token) {
    const token_uint_t new_count = p_line->count + 1;
    token_uint_t new_capacity = p_line->capacity ? p_line->capacity : INIT_TOKEN_CAPACITY;
    while(new_capacity < new_count) new_capacity *= 2;
    if(new_capacity != p_line->capacity)
        if(!(p_line->tokens = realloc((void *)p_line->tokens, sizeof(struct Token) * new_capacity))) return false;
    p_line->capacity = new_capacity; 
    p_line->count = new_count;
    p_line->tokens[new_count - 1] = p_token;
    return true;
}

static bool append_line(struct TokenResult* p_result, const struct TokenLine p_line) {
    const token_uint_t new_count = p_result->count + 1;
    token_uint_t new_capacity = p_result->capacity ? p_result->capacity : INIT_LINE_CAPACITY;
    while(new_capacity < new_count) new_capacity *= 2;
    if(new_capacity != p_result->capacity)
        if(!(p_result->lines = realloc((void *)p_result->lines, sizeof(struct TokenLine) * new_capacity))) return false;
    p_result->capacity = new_capacity; 
    p_result->count = new_count;
    p_result->lines[new_count - 1] = p_line;
    return true;
}

static token_uint_t is_token_number(const struct Token* p_token) {
    if(!p_token->length) return 0;
    enum TokenType type = INT_LIT_E;
    for(token_uint_t i = 0; i < p_token->length; i++) {
        const char c = p_token->string[i];
        if(!isdigit(c)) {
            if(c == '.' && type != FLOAT_LIT_E) type = FLOAT_LIT_E;
            else return 0;
        }
    }
    return type;
}

static void define_token(struct Token* p_token) {
    if (IS_TOKEN_STR(p_token, ",")) p_token->type = COMMA_OP_E;
    else if (IS_TOKEN_STR(p_token, "!")) p_token->type = BANG_OP_E;
    else if (IS_TOKEN_STR(p_token, "@")) p_token->type = AT_OP_E;
    else if (IS_TOKEN_STR(p_token, "/")) p_token->type = SLASH_OP_E;
    else if (IS_TOKEN_STR(p_token, "//#")) p_token->type = INSTRUCT_OP_E;
    else if (IS_TOKEN_STR(p_token, "#")) p_token->type = SHARP_OP_E;
    else if (IS_TOKEN_STR(p_token, ":")) p_token->type = COLON_OP_E;
    else if ((p_token->type = is_token_number(p_token)));
    else p_token->type = STR_LIT_E;
}

static inline bool is_operator(char p_c) {
    return strchr(operator_chars, p_c);
}

static inline bool is_number(char p_c) {
    return isdigit(p_c) || p_c == '.';
}

static bool parse_line(const char* p_string, token_uint_t p_length, struct TokenLine* r_line, struct TokenError* r_error) {
    struct TokenError error = ok;
    struct TokenLine line = {0};
    struct Token token = {0};
    static char indent_char = 0;
    char strlit_char = 0;
    struct CharGroup {
        bool is_reading;
        bool (*is_in_group)(char);
    } groups[] = {
        { false, is_operator},
        { false, is_number}
    };
    bool on_line_start = true;
    for(token_uint_t column = 0; column < p_length; column++) {
        const char c = p_string[column];
        if(on_line_start) {
            if(strchr(indent_chars, c)) {
                if(!indent_char) indent_char = c;
                if(indent_char != c) RAISE_ERROR(error, error_strings.mixed_indentation, on_error);
                line.indent++;
            } else {
                on_line_start = false;
                token.column = column;
                token.string = p_string + column;
                token.length = 0;
                goto parse;
            }
        } else {
            parse:
            if(c == '\\') {
                token.length += 2;
                column++;
                goto next;
            }
            if(strlit_char) {
                token.length++;
                if(c == strlit_char) {
                    strlit_char = 0;
                    APPEND_TOKEN(line, token, error, on_error);
                    token.column = column + 1;
                    token.string = p_string + column + 1;
                    token.length = 0;
                }
                goto next;
            } 
            for (token_uint_t i = 0; i < ARRAY_LEN(groups); i ++) {
                if (groups[i].is_reading) {
                    if(!groups[i].is_in_group(c)) {
                        APPEND_TOKEN(line, token, error, on_error);
                        token.column = column;
                        token.string = p_string + column;
                        token.length = 0;
                        groups[i].is_reading = false;
                        goto parse;
                    } else token.length++;
                    goto next;
                } else if(groups[i].is_in_group(c)) {
                    if(token.length)
                        APPEND_TOKEN(line, token, error, on_error);
                    token.column = column;
                    token.string = p_string + column;
                    token.length = 1;
                    groups[i].is_reading = true;
                    goto next;
                }
            }
            if(strchr(strlit_chars, c)) {
                if(token.length)
                    APPEND_TOKEN(line, token, error, on_error);
                token.column = column + 1;
                token.string = p_string + column;
                token.length = 1;
                strlit_char = c;
                goto next;
            } 
            if (strchr(standalone_chars, c)) {
                if(token.length)
                    APPEND_TOKEN(line, token, error, on_error);
                token.column = column;
                token.string = p_string + column;
                token.length = 1;
                APPEND_TOKEN(line, token, error, on_error);
                token.column = column + 1;
                token.string = p_string + column + 1;
                token.length = 0;
                goto next;
            } 
            if (isspace(c)) {
                if(token.length)
                    APPEND_TOKEN(line, token, error, on_error);
                token.column = column + 1;
                token.string = p_string + column + 1;
                token.length = 0;
                goto next;
            } 
            token.length++;
        }
        next:;
    }
    if(strlit_char) RAISE_ERROR(error, error_strings.unterminated_strlit, on_error);
    if(token.length)
        APPEND_TOKEN(line, token, error, on_error);
    *r_line = line;
    *r_error = ok;
    return true;
    on_error:
    if(line.capacity) free(line.tokens);
    error.column = token.column;
    error.line.string = p_string;
    error.line.length = p_length;
    *r_error = error;
    return false;
}

static void free_result(const struct TokenResult p_result) {
    for(token_uint_t i = 0; i < p_result.count; i++)
        if(p_result.lines[i].capacity) free(p_result.lines[i].tokens);
    if(p_result.capacity) free((void *)p_result.lines);
}

static bool define_line(struct TokenLine* p_line) {
    if(!p_line->capacity) return false;
    if(p_line->tokens[0].type == INSTRUCT_OP_E) {
        p_line->type = INSTRUCT;
        return true;
    }
    if(p_line->count != 3) return false;
    if(
            p_line->tokens[0].type == SHARP_OP_E && 
            IS_TOKEN_STR((p_line->tokens + 1), "include") && 
            strchr(strlit_chars,*p_line->tokens[2].string)
    ) {
        p_line->type = INCLUDE;
        return true;
    }
    return false;
}

static bool parse_text(const char* p_string, struct TokenResult* r_result) {
    struct TokenResult result = {0};
    const char* current = p_string;
    struct {
        const char* string;
        token_uint_t length;
    } text = {p_string, 0};
    token_uint_t row= 0;
    char c = 0;
    while((c = *current)) {
        if(c == '\n') {
            PARSE_LINE(text.string, text.length, row, result, error);
            text.string = current + 1;
            text.length = 0;
            row++;
        } else text.length++;
        current++;
    }
    PARSE_LINE(text.string, text.length, row, result, error);
    *r_result = result;
    return true;
    error:
    free_result(result);
    result.lines = NULL;
    result.capacity = 0;
    result.count = 0;
    result.error.row = row;
    *r_result = result;
    return false;
}

int tokenize(const char* p_string, int (*p_handler)(bool p_success,const struct TokenResult p_result)) {
    struct TokenResult r;
    bool success = parse_text(p_string, &r);
    int status = p_handler(success, r);
    free_result(r);
    return status;
}
