#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "reader.h"
#include "macros.h"
#include "list.h"

typedef int64_t reader_int_t;
typedef uint32_t reader_uint_t;

struct filepath_content_pair_s {
    const char* filepath;
    char* content;
};

LIST_DEFINE_STRUCT(fc_pair, struct filepath_content_pair_s);
LIST_DEFINE_FUNC_GET(fc_pair, struct filepath_content_pair_s, static);
LIST_DEFINE_FUNC_ERASE(fc_pair, struct filepath_content_pair_s, static);
LIST_DEFINE_FUNC_INSERT(fc_pair, struct filepath_content_pair_s, static);
LIST_DEFINE_FUNC_IDENTIFY(fc_pair, struct filepath_content_pair_s, const char*, static);
LIST_DEFINE_FUNC_FREE_ITEMS(fc_pair, struct filepath_content_pair_s, static);

bool fc_finder(const char* p_filepath, const struct filepath_content_pair_s p_pair) {
    return !strcmp(p_filepath, p_pair.filepath);
}

static struct fc_pair_list_s map = {0};

const char* request_content(const char* p_filepath, bool p_reread) {
    struct filepath_content_pair_s pair = {p_filepath, NULL};
    list_uint index = 0;
    if (!fc_pair_list_s_identify(&map, p_filepath, fc_finder, 0, &index)) {
    read_file:;
        FILE* file = NULL;
        long file_size = 0;
        ASSERT_TRUE(file = fopen(p_filepath, "r"), fail);
        ASSERT_FALSE(fseek(file, 0L, SEEK_END), cleanup_file);
        ASSERT_POS(file_size = ftell(file), cleanup_file);
        ASSERT_FALSE(fseek(file, 0L, SEEK_SET), cleanup_file);
        ASSERT_TRUE(pair.content = (char*) calloc(file_size + 1,sizeof(char)), cleanup_content);
        ASSERT_EQU(fread(pair.content, sizeof(char), file_size, file), (size_t) file_size, cleanup_content);
        // Check whether other thread have add content to map while we are
        // reading the file.
        if (!fc_pair_list_s_identify(&map, p_filepath, fc_finder, 0, &index)) {
            ASSERT_TRUE(fc_pair_list_s_append(&map, pair), cleanup_content);
        } else {
            free(pair.content);
            ASSERT_TRUE(fc_pair_list_s_get(&map, index, &pair), fail);
        }
        fclose(file);
        return pair.content;
    cleanup_content:;
        free(pair.content);
    cleanup_file:;
        fclose(file);
        goto fail;
    }
    if(p_reread) {
        fc_pair_list_s_erase(&map, index);
        goto read_file;
    }
    ASSERT_TRUE(fc_pair_list_s_get(&map, index, &pair), fail);
    return pair.content;
fail:;
    return NULL;
}

int reader_read(const char* p_filepath, reader_handler_t p_handler, bool p_reread) {
    int code = 0;
    const char* content = NULL;
    ASSERT_TRUE(content = request_content(p_filepath, p_reread), fail);
    code = p_handler(content);
    return code;
fail:;
    return -1;
}

void reader_clear_cache() {
    fc_pair_list_s_free_items(&map);
}
