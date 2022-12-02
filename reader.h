#ifndef _READER_H_
#define _READER_H_

#include <stdint.h>

typedef uint32_t reader_uint_t;

typedef int (*read_handler_t)(const char* p_content);

int read(const char* p_filepath, read_handler_t p_handler);
#endif //_READER_H_
