#ifndef _READER_H_
#define _READER_H_

#include <stdint.h>

typedef uint32_t reader_uint_t;

typedef int (*reader_handler_t)(const char* p_content);

int reader_read(const char* p_filepath, reader_handler_t p_handler);
#endif //_READER_H_
