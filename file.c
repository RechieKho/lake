#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file.h"

#define INIT_CONTENT_CAPACITY 100

static bool append_to_content(struct ReadFileContent* p_content, char p_c) {
    const file_uint_t new_length = p_content->length + 1;
    file_uint_t new_capacity = p_content->capacity ? p_content->capacity : INIT_CONTENT_CAPACITY;
    while(new_capacity < new_length) new_capacity *= 2;
    if(new_capacity != p_content->capacity)
        if(!(p_content->string = (char*)realloc(p_content->string, sizeof(char)*new_capacity))) return false;
    p_content->capacity = new_capacity;
    p_content->length = new_length;
    p_content->string[new_length - 1] = p_c;
    return true;
}

static void free_content(const struct ReadFileContent p_content) {
    if(p_content.capacity) free(p_content.string);
}

int read_file(const char* p_filepath, read_file_handler_t p_handler) {
    struct ReadFileContent content = {0};
    int c = 0;
    int status = -1;
    FILE* file;
    if(!(file = fopen(p_filepath, "r"))) return status;
    while ((c = fgetc(file)) != EOF) if(!append_to_content(&content, c)) goto error;
    if(!append_to_content(&content, 0)) goto error;
    status = p_handler(content);
error:
    fclose(file);
    free_content(content);
    return status;
}
