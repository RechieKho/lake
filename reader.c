#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "reader.h"

#define INIT_CONTENT_CAPACITY 100
#define ASSERT_TRUE(mp_expression, mp_label) if(!(mp_expression)) goto mp_label
#define ASSERT_FALSE(mp_expression, mp_label) if((mp_expression)) goto mp_label
#define ASSERT_POS(mp_expression, mp_label) if((mp_expression) < 0) goto mp_label
#define ASSERT_EQU(mp_expression, mp_value, mp_label) if((mp_expression) != mp_value) goto mp_label
#define COUNT_UP(mp_type, mp_variable, mp_count) for(mp_type mp_variable = 0; mp_variable < mp_count; mp_variable++)

int read(const char* p_filepath, read_handler_t p_handler) {
    int code = -1;
    size_t file_size = 0;
    char* content = NULL;
    FILE* file = fopen(p_filepath, "r");
    ASSERT_TRUE(file, quit);
    ASSERT_FALSE(fseek(file, 0L, SEEK_END), cleanup_file);
    ASSERT_POS(file_size = ftell(file), cleanup_file);
    ASSERT_FALSE(fseek(file, 0L, SEEK_SET), cleanup_file);
    ASSERT_TRUE(content = (char*) calloc(file_size + 1,sizeof(char)), cleanup_content);
    ASSERT_EQU(fread(content, sizeof(char), file_size, file), file_size, cleanup_content);
    code = p_handler(content);
    return code;
cleanup_content:
    free(content);
cleanup_file:
    fclose(file);
quit:
    return code;
}
