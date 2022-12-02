#ifndef _LIST_H_
#define _LIST_H_

/* # list
 * This file contains macro for declaring and defining list structure that
 * stores desired type. It behaves like template in C++, but the programmer need
 * to declare or define it manually. 
 *
 * ## Usage 
 * 1. Declare & define list structure & functions with the macros (read
 * ## Declaration and defintion)
 * 2. Initialize the list structure (read ## Memory). 
 * 3. Use the list with the function declared / defined
 * (read below for functions' documentation)
 * 4. Free the list (read ## Memory). 
 *
 * Each function has its own declaration macro. 
 * But for definition macro, some functions are grouped together.
 *
 * ## Declaration and defintion
 * `LIST_DECLARE_*` declares stuff while `LIST_DEFINE_*` define implementation. 
 * `mp_id` will be the prefix of the functions' & structure's identifier. 
 * `mp_type` will be the type of the item stored. 
 * `mp_keyword` will be the keyword that for the structure and / or functions
 * (e.g. `static`, `inline`).
 *
 * ## Memory
 * The array that stores the items will always be on the heap, while the
 * list structure itself (that stores pointer to the array and etc.) can be on
 * the stack or heap. 
 *
 * For list on heap, use `ID_list_new` to create a list on heap. `ID_list_free`
 * must be used to free the list and its array. 
 *
 * For list on stack, just declare a variable with type of `struct ID_list` and
 * you are good to go. `ID_list_free_items` must be used to free the list's
 * array.
 *
 * To avoid memory leaks:
 * - DON'T directly modify the member of the struct, unless you know want you are doing. 
 * - DON'T Free with `free` from stdlib instead of `ID_list_free`.
 * - DON'T Give NULL pointer as argument, all functions do not check the validity of
 *   pointer.
 * */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint32_t list_uint;

#ifndef LIST_INIT_ITEM_COUNT
#define LIST_INIT_ITEM_COUNT 20
#endif //LIST_INIT_ITEM_COUNT

/* # list structure 
 * >> struct ID_list 
 *
 * @member 
 *  `capacity` - Number of allocated slots. 
 *  `length` - Number of stored items. 
 *  `items` - Array of items. 
 * <<
 * */
#define LIST_DECLARE_STRUCT(mp_id, mp_keyword) \
    mp_keyword struct mp_id ## _list; 

/* Get item by index.
 *
 * @param
 *  `p_list` - The list to be operated on.
 *  `p_index` - Index of item retreived. 
 *
 * @return
 *  `r_item` - Retreived item.
 * */
#define LIST_DECLARE_FUNC_GET(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_get(const struct mp_id ## _list* p_list, list_uint p_index, mp_type* r_item);

/* Get the length of list.
 *
 * @param
 *  `p_list` - The list to be operated on.
 *
 * @return 
 *  % - Length of list.
 *
 * @noerror
 * */
#define LIST_DECLARE_FUNC_LENGTH(mp_id, mp_type, mp_keyword) \
    mp_keyword list_uint mp_id ## _list_length(const struct mp_id ##_list* p_list);

/* Find the index of nth item given. 
 *
 * @param 
 *  `p_list` - The list to be operated on.
 *  `p_item` - Item to find. 
 *  `p_nth` - Nth item.
 *
 * @return 
 *  `r_index` - index of the item in the list.
 *
 * @error 
 *  | When the item is not in the list, it fails.
 *  % - `true` on success. `false` on fail.
 * */
#define LIST_DECLARE_FUNC_FIND(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_find(const struct mp_id ## _list* p_list, const mp_type p_item, list_uint p_nth, list_uint* r_index);

/* Check whether the lists given are equal.
 *
 * @param 
 *  `p_list_a` - First list. 
 *  `p_list_b` - Second list. 
 *
 * @return 
 *  % - `true` if equal, else false. 
 *
 * @noerror
 * */
#define LIST_DECLARE_FUNC_EQUAL(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_equal(const struct mp_id ## _list* p_list_a, const struct mp_id ## _list* p_list_b);

/* Allocate memory for the list structure.
 * 
 * @noparam
 *
 * @return 
 *  % - Pointer to the allocated list structure. 
 *
 * @error 
 *  | When the allocator function fails, it fails.
 *  % - Valid pointer on success. `NULL` on fail. 
 * */
#define LIST_DECLARE_FUNC_NEW(mp_id, mp_type, mp_keyword) \
    mp_keyword struct mp_id ## _list* mp_id ## _list_new();

/* Free the list structure together with the array. 
 *  It only free the array when `capacity` is not 0. 
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *
 * @noreturn 
 *
 * @noerror
 * */
#define LIST_DECLARE_FUNC_FREE(mp_id, mp_type, mp_keyword) \
    mp_keyword void mp_id ## _list_free(struct mp_id ## _list* p_list);

/* Free the array allocated when `capacity` is not 0. 
 *
 * @param
 *  `p_list` - The list to be operated. 
 *
 * @noreturn
 * 
 * @noerror
 * */
#define LIST_DECLARE_FUNC_FREE_ITEMS(mp_id, mp_type, mp_keyword) \
    mp_keyword void mp_id ## _list_free_items(struct mp_id ## _list* p_list);

/* Copy an array to the list. 
 *
 * @param 
 *  `p_array` - Array to be copied from. 
 *  `p_length` - Length of the array. 
 * 
 * @return 
 *  `r_list` - Target list to be copied to. 
 *
 * @error
 *  | When fail to allocate an array, it fails.
 *  % - `true` on success. `false` on fail. 
 * */
#define LIST_DECLARE_FUNC_FROM_ARRAY(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_from_array(const mp_type* p_array, list_uint p_length, struct mp_id ## _list* r_list);

/* Set item in the list by index. 
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *  `p_item` - New item. 
 *  `p_index` - Index of item to be set. 
 *
 * @noreturn 
 *
 * @error 
 *  | When the index is out of bound, it fails. 
 *  % - `true` on success. `false` on fail. 
 * */
#define LIST_DECLARE_FUNC_SET(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_set(struct mp_id ## _list* p_list, const mp_type p_item, list_uint p_index);

/* Insert item into the list and push back items behind.
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *  `p_item` - New item. 
 *  `p_index` - Index of item to be set. 
 *
 * @noreturn 
 *
 * @error 
 *  | When the index is out of bound, it fails. 
 *  | When fail to allocate an array, it fails.
 *  % - `true` on success. `false` on fail. 
 * */
#define LIST_DECLARE_FUNC_INSERT(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_insert(struct mp_id ## _list* p_list, const mp_type p_item, list_uint p_index);

/* Remove item from the list and push front the items behind.
 *
 * @param
 *  `p_list` - The list to be operated. 
 *  `p_index` - Index of item to be set. 
 *
 * @noreturn 
 *
 * @error 
 *  | When the index is out of bound, it fails. 
 *  % - `true` on success. `false` on fail. 
 * */
#define LIST_DECLARE_FUNC_ERASE(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_erase(struct mp_id ## _list* p_list, list_uint p_index);

/* Append item at the back of the list.
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *  `p_item` - New item. 
 *
 * @noreturn 
 *
 * @error 
 *  | When `ID_list_insert` fails, it fails.
 *  % - `true` on succes. `false` on fail.
 * */
#define LIST_DECLARE_FUNC_APPEND(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_append(struct mp_id ## _list* p_list, const mp_type p_item);

/* Prepend item at the front of the list.
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *  `p_item` - New item. 
 *
 * @noreturn 
 *
 * @error 
 *  | When `ID_list_insert` fails, it fails.
 *  % - `true` on succes. `false` on fail.
 * */
#define LIST_DECLARE_FUNC_PREPEND(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_prepend(struct mp_id ## _list* p_list, const mp_type p_item);

/* Remove item at the back of the list.
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *
 * @return 
 *  `r_popped` - Item that popped. 
 *
 * @error 
 *  | When `ID_list_erase` fails, it fails. 
 *  % - `true` on succes. `false` on fail.
 * */
#define LIST_DECLARE_FUNC_POP_FRONT(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_pop_front(struct mp_id ## _list* p_list, mp_type* r_popped);

/* Remove item at the front of the list.
 *
 * @param 
 *  `p_list` - The list to be operated. 
 *
 * @return 
 *  `r_popped` - Item that popped. 
 *
 * @error 
 *  | When `ID_list_erase` fails, it fails. 
 *  % - `true` on succes. `false` on fail.
 * */
#define LIST_DECLARE_FUNC_POP_BACK(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_pop_back(struct mp_id ## _list* p_list, mp_type* r_popped);

#define LIST_DEFINE_STRUCT(mp_id, mp_type, mp_keyword) \
    mp_keyword struct mp_id ## _list { \
        list_uint capacity; \
        list_uint length; \
        mp_type* items; \
    }

#define LIST_DEFINE_FUNC_GET(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_get(const struct mp_id ## _list* p_list, list_uint p_index, mp_type* r_item) { \
        if(p_index >= p_list->length) return false;  \
        if(r_item) *r_item = p_list->items[p_index];  \
        return true; \
    }

#define LIST_DEFINE_FUNC_LENGTH(mp_id, mp_type, mp_keyword) \
    mp_keyword list_uint mp_id ## _list_length(const struct mp_id ##_list* p_list) { \
        return p_list->length; \
    }

#define LIST_DEFINE_FUNC_FIND(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_find(const struct mp_id ## _list* p_list, const mp_type p_item, list_uint p_nth, list_uint* r_index) { \
        for(list_uint i = 0; i < p_list->length; i++) \
            if(p_list->items[i] == p_item && !p_nth--) { \
                *r_index = i;  \
                return true;  \
            } \
        return false; \
    }

#define LIST_DEFINE_FUNC_EQUAL(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_equal(const struct mp_id ## _list* p_list_a, const struct mp_id ## _list* p_list_b) { \
        if(p_list_a->length != p_list_b->length) return false; \
        for(list_uint i = 0; i < p_list_a->length; i++) \
            if(p_list_a->items[i] != p_list_b->items[i]) return false;  \
        return true; \
    }

#define LIST_DEFINE_FUNC_NEW(mp_id, mp_type, mp_keyword) \
    mp_keyword struct mp_id ## _list* mp_id ## _list_new() { \
        return calloc(1, sizeof(struct mp_id ## _list)); \
    }

#define LIST_DEFINE_FUNC_FREE(mp_id, mp_type, mp_keyword) \
    mp_keyword void mp_id ## _list_free(struct mp_id ## _list* p_list) { \
        if(p_list->capacity) free(p_list->items);  \
        free(p_list);  \
    }

#define LIST_DEFINE_FUNC_FREE_ITEMS(mp_id, mp_type, mp_keyword) \
    mp_keyword void mp_id ## _list_free_items(struct mp_id ## _list* p_list) { \
        if(p_list->capacity) free(p_list->items); \
    }

#define LIST_DEFINE_FUNC_SET(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_set(struct mp_id ## _list* p_list, const mp_type p_item, list_uint p_index) { \
        if(p_index >= p_list->length) return false;  \
        p_list->items[p_index] = p_item;  \
        return true; \
    }

#define LIST_DEFINE_FUNC_INSERT(mp_id, mp_type, mp_keyword) \
    static bool mp_id ## _list_make_space(struct mp_id ## _list* p_list, list_uint p_new_length) { \
        list_uint new_capacity = p_list->capacity? p_list->capacity: LIST_INIT_ITEM_COUNT; \
        while(new_capacity < p_new_length) new_capacity *= 2;  \
        mp_type* new_items = NULL; \
        if(!(new_items = realloc(p_list->items, new_capacity * sizeof(mp_type)))) return false;  \
        p_list->items = new_items;  \
        p_list->capacity = new_capacity;  \
        return true; \
    } \
    mp_keyword bool mp_id ## _list_insert(struct mp_id ## _list* p_list, const mp_type p_item, list_uint p_index) { \
        if(p_index > p_list->length) return false;  \
        if(!mp_id ## _list_make_space(p_list, p_list->length + 1)) return false; \
        for(list_uint i = p_list->length; i > p_index; i--) \
            memcpy(p_list->items + i, p_list->items + i - 1, sizeof(mp_type));   \
        p_list->items[p_index] = p_item; \
        p_list->length++; \
        return true;  \
    } \
    mp_keyword bool mp_id ## _list_append(struct mp_id ## _list* p_list, const mp_type p_item) { \
        return mp_id ## _list_insert(p_list, p_item, p_list->length); \
    } \
    mp_keyword bool mp_id ## _list_prepend(struct mp_id ## _list* p_list, const mp_type p_item) { \
        return mp_id ## _list_insert(p_list, p_item, 0); \
    } \
    mp_keyword bool mp_id ## _list_from_array(const mp_type* p_array, list_uint p_length, struct mp_id ## _list* r_list) { \
        if(!mp_id ## _list_make_space(r_list, p_length)) return false; \
        memcpy(r_list->items, p_array, p_length * sizeof(mp_type));  \
        r_list->length = p_length;  \
        return true;  \
    } \

#define LIST_DEFINE_FUNC_ERASE(mp_id, mp_type, mp_keyword) \
    mp_keyword bool mp_id ## _list_erase(struct mp_id ## _list* p_list, list_uint p_index) { \
        if(p_index >= p_list->length) return false;  \
        for(list_uint i = p_index; i < p_list->length - 1; i++) \
            memcpy(p_list->items + i, p_list->items + i + 1, sizeof(mp_type));   \
        p_list->length--;  \
        return true;  \
    } \
    mp_keyword bool mp_id ## _list_pop_back(struct mp_id ## _list* p_list, mp_type* r_popped) { \
        const mp_type popped = p_list->items[p_list->length];  \
        if(!mp_id ## _list_erase(p_list, p_list->length - 1)) return false; \
        *r_popped = popped;  \
        return true; \
    } \
    mp_keyword bool mp_id ## _list_pop_front(struct mp_id ## _list* p_list, mp_type* r_popped) { \
        const mp_type popped = p_list->items[0]; \
        if(!mp_id ## _list_erase(p_list, 0)) return false; \
        *r_popped = popped;  \
        return true; \
    }

#endif //_LIST_H_
