#ifndef _FILE_H_
#define _FILE_H_

#include <stdint.h>

typedef uint32_t file_uint_t;

struct ReadFileContent {
    char* string;
    file_uint_t length;
    file_uint_t capacity;
};

typedef int (*read_file_handler_t)(const struct ReadFileContent p_content);

int read_file(const char* p_filepath, read_file_handler_t p_handler);
#endif //_FILE_H_
