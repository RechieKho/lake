#include <stdio.h>
#include "reader.h"
#include "token.h"
#include "errno.h"

static void print_token_result(const struct TokenResult p_result) {
    for(token_uint_t i = 0; i < p_result.count; i++) {
        const struct TokenLine line = p_result.lines[i];
        printf("Line %u (indent %u, type %u):\n", line.row, line.indent, line.type);
        for(token_uint_t j = 0; j < line.count; j++) {
            const struct Token token = line.tokens[j];
            const char* string = token.string;
            printf("\t|");
            for(token_uint_t k = 0; k < token.length; k++) 
                putc(*(string++), stdout);
            printf("| (column %u, type %i)\n", token.column, token.type);
        }
    }
}

static int token_handler(bool p_success, const struct TokenResult p_result) {
    if(!p_success) {
        printf("Error: %s. [row %u, column %u]\n", p_result.error.message, p_result.error.row, p_result.error.column);
        return 1;
    }
    print_token_result(p_result);
    return 0;
}

static int read_handler(const char* p_content){
    tokenize(p_content, token_handler);
    return 0;
};

int main(int argc, const char* argv[]) {
    if(argc < 2) return 0;
    printf("Filepath: %s\n", argv[1]);
    int status = read(argv[1], read_handler);
    if(status < 0) fprintf(stderr, "Error: %d code.\n", errno);
    return status;
}
