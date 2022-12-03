#ifndef _READER_H_
#define _READER_H_

#include <stdbool.h>

typedef int (*reader_handler_t)(const char* p_content);

int reader_read(const char* p_filepath, reader_handler_t p_handler, bool p_reread);
void reader_clear_cache();
#endif //_READER_H_
