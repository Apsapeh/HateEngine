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
} StringSlice;

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
 * @brief check on equality Strings
 * @param str1 fisrt String
 * @param str2 second String
 */
boolean string_equals(const String* str1, const String* str2);

/**
 * @api
 * @brief check on equality String and char*
 * @param str checking String
 * @param c_str checking char*
 */
boolean string_equals_cstr(const String* str, const char* c_str);

/**
 * @api
 * @brief free memory of passed String
 */
void string_free(String* self);

/**
 * @api
 * @brief get StringSlice from String
 * @param s start point
 * @param e end point
 */
StringSlice* string_get_slice(const String* self, const usize s, const usize e);

/**
 * @api
 * @brief String from StringSlice
 */
String* string_from_slice(const StringSlice* self);

/**
 * @api
 * @brief check on equality StringsSlice
 * @param str_sl_1 fisrt StringSlice
 * @param str_sl_2 second StringSlice
 */
boolean string_equals_slice(StringSlice* str_sl_1, StringSlice* str_sl_2);

/**
 * @api
 * @brief copy data str_sl in self
 * @param str_sl StringSLice which from take data
 */
StringSlice* string_set_slice(StringSlice* self, const StringSlice* str_sl);

/**
 * @api
 * @brief add StringSlice in last of self-String
 * @param src StringSlice that will be added
 */
String* string_push_back_slice(String* self, const StringSlice* src);

/**
 * @api
 * @brief add StringSlice in front of self-String
 * @param src StringSlice that will be added
 */
String* string_push_front_slice(String* self, const StringSlice* src);

String* string_insert_slice_ex(String* self, const StringSlice* src, const usize i);

/**
 * @api
 * @brief insert StringSlice in String under symbol index i
 * @param src StringSlice that will be inserted
 * @param i   symbol index
 */
String* string_insert_slice(String* self, const StringSlice* src, const usize i);

/**
 * @api
 * @brief insert StringSlice in String under byte index b\
 * @param src StringSlice that will be inserted
 * @param b   byte index
 */
String* string_insert_slice_by_byte(String* self, const StringSlice* src, const usize b);

/**
 * @api
 * @brief free memory of passed StringSLice
 */
void string_slice_free(StringSlice* self);

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
