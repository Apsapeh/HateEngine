#pragma once

#include "error.h"
#include "types.h"


/**
 * @api
 */
typedef struct {
    u8* ptr;
    usize len;
} String;

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
 * @brief create empty String with null-terminator
 */
String* string_new(void);

/**
 * @api
 * @brief create String by char*
 */
String* string_from(const char* c_str);

/**
 * @api
 * @brief clone String by existing String
 */
String* string_clone(const String* c_str);

/**
 * @api
 * @brief update String by char* knowing its len
 * @param c_str pointer on new content of String
 * @param len_c_str len of c_str
 */
String* string_set_cstr_len(String* self, const char* c_str, const usize len_c_str);

/**
 * @api
 * @brief update String by char*
 * @param c_str pointer on new content of String
 */
String* string_set_cstr(String* self, const char* c_str);

/**
 * @api
 * @brief update String by existing String
 * @param str  copied String
 */
String* string_set(String* self, const String* str);

/**
 * @api
 * @brief get String by way char*
 */
char* string_cstr(const String* self);

/**
 * @api
 * @brief get len of the passed String
 */
usize string_len(const String* c_str);

/**
 * @api
 * @brief get size of the passed String
 */
usize string_size(const String* c_str);

/**
 * @api
 * @brief add src-String in the end of self-String
 * @param src  String that need to add
 */
String* string_push_back(String* self, const String* src);

/**
 * @api
 * @brief add  char* in the end of self-String
 * @param src  char* that need to add
 */
String* string_push_back_cstr(String* self, const char* src);

String* string_push_back_cstr_ex(String* self, const char* src);

/**
 * @api
 * @brief add  src-String in the front of self-String
 * @param src  String that need to add
 */
String* string_push_front(String* self, const String* src);

/**
 * @api
 * @brief add  char* in the front of self-String
 * @param src  char* that need to add
 */
String* string_push_front_cstr(String* self, const char* src);

String* string_push_front_cstr_ex(String* self, const char* src);

/**
 * @api
 * @brief insert src-String in self-String by symbol index
 * @param src String that need to insert
 * @param i   symbol index
 */
String* string_insert(String* self, const String* src, const usize i);

/**
 * @api
 * @brief insert char* in self-String by symbol index
 * @param src char* that need to insert
 * @param i   symbol index
 */
String* string_insert_cstr(String* self, const char* src, const usize i);

String* string_insert_cstr_ex(String* self, const char* src, const usize i, const usize len_src);

/**
 * @api
 * @brief insert char* in self-String by byte index
 * @param src char* that need to insert
 * @param b   byte index
 */
String* string_insert_cstr_by_byte(String* self, const char* src, const usize b);

/**
 * @api
 * @brief insert src-String in self-String by byte index
 * @param src String that need to insert
 * @param b   byte index
 */
String* string_insert_by_byte(String* self, const String* src, const usize b);

/**
 * @api
 * @brief remove symbol under symbol index
 * @param i symbol index
 */
String* string_remove(String* self, const usize i);

/**
 * @api
 * @brief remove symbol under byte index
 * @param b byte index
 */
String* string_remove_by_byte(String* self, const usize b);

/**
 * @api
 * @brief remove n of symbols since symbol  under symbol index
 * @param i symbol index
 * @param n count removed symbols
 */
String* string_remove_n(String* self, const usize i, const usize n);

/**
 * @api
 * @brief remove n of byte since byte  under byte index
 * @param b byte index
 * @param b_n count removed byte
 */
String* string_remove_n_by_byte(String* self, const usize b, const usize b_n);

/**
 * @api
 */
// test
boolean string_equals(const String* str1, const String* str2);

/**
 * @api
 */
// test
boolean string_equals_cstr(const String* str, const char* cstr);

/**
 * @api
 */
void string_free(String* str);

/**
 * @api
 */
void string_itr_free(string_itr* str_itr);

/**
 * @api
 */
string_itr* string_get_itr(const String* c_str);

/**
 * @api
 */
u8 string_itr_next(string_itr* str_itr);

/**
 * @api
 */
string_slice* string_get_slice(const String* str, const usize s, const usize e);

/**
 * @api
 */
String* string_from_slice(const string_slice* str_sl);

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
String* string_push_back_slice(String* dest, const string_slice* src);

/**
 * @api
 */
// test
String* string_push_front_slice(String* dest, const string_slice* src);

/**
 * @api
 */
// test
String* string_insert_slice_ex(String* dest, const string_slice* src, const usize i);

/**
 * @api
 */
// test
String* string_insert_slice(String* dest, const string_slice* src, const usize i);

/**
 * @api
 */
// test
String* string_insert_slice_by_byte(String* dest, const string_slice* src, const usize b);

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
Error string_utf8_to_string(String** dest, const string_utf8* str);

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
