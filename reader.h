#ifndef _READER_H_
#define _READER_H_

#include <stdint.h>

typedef uint32_t reader_uint_t;

struct ReadContent {
    char* string;
    reader_uint_t length;
    reader_uint_t capacity;
};

typedef int (*read_handler_t)(const struct ReadContent p_content);

int read(const char* p_filepath, read_handler_t p_handler);
#endif //_READER_H_
