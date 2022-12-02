#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "reader.h"
#include "macros.h"

#define INIT_CONTENT_CAPACITY 100

int read(const char* p_filepath, read_handler_t p_handler) {
    int code = -1;
    long file_size = 0;
    char* content = NULL;
    FILE* file = fopen(p_filepath, "r");
    ASSERT_TRUE(file, quit);
    ASSERT_FALSE(fseek(file, 0L, SEEK_END), cleanup_file);
    ASSERT_POS(file_size = ftell(file), cleanup_file);
    ASSERT_FALSE(fseek(file, 0L, SEEK_SET), cleanup_file);
    ASSERT_TRUE(content = (char*) calloc(file_size + 1,sizeof(char)), cleanup_content);
    ASSERT_EQU(fread(content, sizeof(char), file_size, file), (size_t) file_size, cleanup_content);
    code = p_handler(content);
    return code;
cleanup_content:
    free(content);
cleanup_file:
    fclose(file);
quit:
    return code;
}
