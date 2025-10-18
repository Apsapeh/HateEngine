#pragma once

#include "error.h"
#include "types.h"


/**
 * @api
 */
typedef struct {
    u8* ptr;
    usize len;
} string;

/**
 * @api
 */
typedef struct {
    const u8* start;
    const u8* cur;
    usize size;
} string_itr;

/**
 * @api
 */
typedef struct {
    u8* s;
    usize len;
} string_slice;

/**
 * @api
 */
Error string_new(string** str);

/**
 * @api
 */
string* string_from(const char* c_str);

/**
 * @api
 */
string* string_clone(const string* c_str);

/**
 * @api
 */
string* string_set_cstr(string* self, const char* c_cstr);

/**
 * @api
 */
string* string_set(string* self, const string* c_str);

/**
 * @api
 */
// test
char* string_cstr(const string* c_str);

/**
 * @api
 */
// test
usize string_len(const string* c_str);

/**
 * @api
 */
// test
usize string_size(const string* c_str);

/**
 * @api
 */
// test
string* string_push_back(string* dest, const string* src);

/**
 * @api
 */
// test
string* string_push_back_cstr(string* dest, const char* src);

/**
 * @api
 */
// test
string* string_push_front(string* dest, const string* src);

/**
 * @api
 */
// test
string* string_push_front_cstr(string* dest, const char* src);

/**
 * @api
 */
// test
string* string_insert(string* dest, const string* src, const usize i);

/**
 * @api
 */
// test
string* string_insert_cstr(string* dest, const char* src, const usize i);

/**
 * @api
 */
// test
string* string_insert_cstr_ex(string* dest, const char* src, const usize i);

/**
 * @api
 */
// test
string* string_insert_cstr_by_byte(string* dest, const char* src, const usize b);

/**
 * @api
 */
// test
string* string_insert_by_byte(string* dest, const string* src, const usize b);

/**
 * @api
 */
// test
string* string_remove(string* self, const usize i);

/**
 * @api
 */
// test
string* string_remove_by_byte(string* self, const usize b);

/**
 * @api
 */
// test
string* string_remove_n(string* self, const usize i, const usize n);

/**
 * @api
 */
// test
string* string_remove_n_by_byte(string* self, const usize b, const usize b_n);

/**
 * @api
 */
// test
boolean string_equals(const string* str1, const string* str2);

/**
 * @api
 */
// test
boolean string_equals_cstr(const string* str, const char* cstr);

/**
 * @api
 */
void string_free(string* str);

/**
 * @api
 */
void string_itr_free(string_itr* str_itr);

/**
 * @api
 */
string_itr* string_get_itr(const string* c_str);

/**
 * @api
 */
u8 string_itr_next(string_itr* str_itr);

/**
 * @api
 */
string_slice* string_get_slice(const string* str, const usize s, const usize e);

/**
 * @api
 */
string* string_from_slice(const string_slice* str_sl);

/**
 * @api
 */
boolean string_equals_slice(string_slice* str_sl_1, string_slice* str_sl_2);

/**
 * @api
 */
string_slice* string_set_slice(string_slice* self, const string_slice* str_sl);

/**
 * @api
 */
// test
string* string_push_back_slice(string* dest, const string_slice* src);

/**
 * @api
 */
// test
string* string_push_front_slice(string* dest, const string_slice* src);

/**
 * @api
 */
// test
string* string_insert_slice_ex(string* dest, const string_slice* src, const usize i);

/**
 * @api
 */
// test
string* string_insert_slice(string* dest, const string_slice* src, const usize i);

/**
 * @api
 */
// test
string* string_insert_slice_by_byte(string* dest, const string_slice* src, const usize b);

/**
 * @api
 */
void string_slice_free(string_slice* str_sl);

//<--------------------------- UTF-8 --------------------------->

/**
 * @api
 */
typedef struct {
    u32* ptr;
    usize len;
} string_utf8;

/**
 * @api
 */
typedef struct {
    u8* start;
    usize len;
} string_itr_utf8;

static Error string_utf8_dec(string_utf8** str, const u8* c_str);

/**
 * @api
 */
Error string_utf8_new(string_utf8** str);

/**
 * @api
 */
Error string_utf8_to_string(string** dest, const string_utf8* str);

/**
 * @api
 */
Error string_utf8_from(string_utf8** str, const char* c_str);

/**
 * @api
 */
// test
Error string_utf8_len(usize* len, const string_utf8* c_str);

/**
 * @api
 */
// test
Error string_utf8_size(usize* size, const string_utf8* c_str);

/**
 * @api
 */
Error string_utf8_clone(string_utf8** str, const string_utf8* c_str);

/**
 * @api
 */
// test
Error string_utf8_push_back_cstr(string_utf8* dest, const char* src);

/**
 * @api
 */
// test
Error string_utf8_push_back(string_utf8* dest, const string_utf8* src);

/**
 * @api
 */
// test
Error string_utf8_insert(string_utf8* dest, const string_utf8* src, const usize i);

/**
 * @api
 */
// test
Error string_utf8_push_front(string_utf8* dest, const string_utf8* src);

/**
 * @api
 */
void string_utf8_free(string_utf8* str);
