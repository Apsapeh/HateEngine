#include "String.h"
#include <String.h>
#include "error.h"
#include "types.h"
#include "../platform/memory.h"
#include "../log.h"

String* string_new(void) {
    String* str_new = tmalloc(sizeof(String));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    str_new->ptr = tmalloc(sizeof(u8));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    *(str_new->ptr) = '\0';
    str_new->len = 0;

    return str_new;
}

String* string_from(const char* c_str) {
    ERROR_ARG_CHECK(c_str, { return NULL; });

    String* str = tmalloc(sizeof(String));
    ERROR_ALLOC_CHECK(str, { return NULL; });

    usize c_str_len = strlen(c_str);
    str->ptr = tmalloc(c_str_len + 1);
    ERROR_ALLOC_CHECK(str->ptr, { 
        tfree(str);
        return NULL; 
    });

    str->len = c_str_len;
    memcpy(str->ptr, c_str, c_str_len + 1);

    return str;
}

String* string_clone(const String* str) {
    ERROR_ARG_CHECK(str, { return NULL; });

    String* str_new = tmalloc(sizeof(String));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    str_new->ptr = tmalloc(str->len + 1);
    ERROR_ALLOC_CHECK(str_new->ptr, { 
        tfree(str_new);
        return NULL; 
    });

    str_new->len = str->len;
    memcpy(str_new->ptr, str->ptr, str->len + 1);

    return str_new;
}

String* string_set_cstr_len(String* self, const char* c_str, const usize len_c_str) {
    ERROR_ARGS_CHECK_2(self, c_str, { return NULL; });

    u8* tmp_ptr = trealloc(self->ptr, len_c_str + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    self->len = len_c_str;
    memcpy(self->ptr, c_str, len_c_str + 1);

    return self;
}

String* string_set_cstr(String* self, const char* c_str) {
    return string_set_cstr_len(self, c_str, strlen(c_str));
}

String* string_set(String* self, const String* str) {
    return string_set_cstr_len(self, (char*) str->ptr, str->len);
}

char* string_cstr(const String* self) {
    ERROR_ARG_CHECK(self, { return NULL; });
    return (char*) self->ptr;
}

usize string_len(const String* c_str) {
    return c_str->len;
}

usize string_size(const String* c_str) {
    return string_len(c_str) + 1;
}

String* string_push_back(String* self, const String* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });
    return string_push_back_cstr_ex(self, (char*) src->ptr);
}

String* string_push_back_cstr(String* self, const char* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });
    return string_push_back_cstr_ex(self, src);
}

String* string_push_back_cstr_ex(String* self, const char* src) {
    usize len = strlen(src);
    u8* tmp_ptr = trealloc(self->ptr, self->len + len + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + self->len, src, len + 1);
    self->len += len;

    return self;
}

String* string_push_front(String* self, const String* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });
    return string_push_front_cstr(self, (char*) src->ptr);
}

String* string_push_front_cstr(String* self, const char* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });
    return string_push_front_cstr_ex(self, src);
}

String* string_push_front_cstr_ex(String* self, const char* src) {
    usize len = strlen(src);
    u8* tmp_ptr = trealloc(self->ptr, self->len + len + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + len, self->ptr, self->len + 1);
    memcpy(self->ptr, src, len);
    self->len += len;

    return self;
}

String* string_insert(String* self, const String* src, const usize i) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (i < 0 || i >= len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    return string_insert_cstr_ex(self, (char*) src->ptr, i, strlen((char*) src->ptr));
}

String* string_insert_cstr(String* self, const char* src, const usize i) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (i < 0 || i >= len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    return string_insert_cstr_ex(self, src, i, len);
}

String* string_insert_cstr_by_byte(String* self, const char* src, const usize b) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (b < 0 || b > len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    return string_insert_cstr_ex(self, src, b, len);
}

String* string_insert_cstr_ex(String* self, const char* src, const usize i, const usize len_src) {
    if (i == 0)
        return string_push_front_cstr(self, src);

    usize len = self->len;
    u8* tmp_ptr = trealloc(self->ptr, self->len + len_src + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + len_src + i, self->ptr + i, self->len - i + 1);
    memcpy(self->ptr + i, src, len_src);
    self->len += len_src;

    return self;
}

String* string_insert_by_byte(String* self, const String* src, const usize b) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (b < 0 || b > len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    return string_insert_cstr_ex(self, (char*) src->ptr, b, len);
}

String* string_remove(String* self, const usize i) {
    ERROR_ARG_CHECK(self, { return NULL; });

    if (i < 0 || i >= self->len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    u8* tmp_ptr = trealloc(self->ptr, self->len);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    memcpy(self->ptr + i, self->ptr + i + 1, self->len - i);
    self->len -= 1;
    *(self->ptr + self->len + 1) = '\0';

    return self;
}

String* string_remove_by_byte(String* self, const usize b) {
    return string_remove(self, b);
}

String* string_remove_n(String* self, const usize i, const usize n) {
    if (n == 0)
        return string_remove(self, i);

    ERROR_ARG_CHECK(self, { return NULL; });

    if (i < 0 || i >= self->len || n < 0 || i + n >= self->len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    u8* tmp_ptr;
    
    if (i + n == self->len) {
        u8* tmp_ptr = trealloc(self->ptr, i);
        if (!tmp_ptr)
            return NULL;

        *(tmp_ptr + i) = '\0';
    } else {
        usize len_cache = self->len - i - n + 1;
        u8 cache[len_cache];
        memcpy(cache, self->ptr + i + n + 1, len_cache);
        u8* tmp_ptr = trealloc(self->ptr, i);
        if (!tmp_ptr)
            return NULL;

        memcpy(tmp_ptr + i, cache, len_cache);
    }

    self->ptr = tmp_ptr;
    self->len -= n + 1;
    return self;
}

String* string_remove_n_by_byte(String* self, const usize b, const usize b_n) {
    return string_remove_n(self, b, b_n);
}

boolean string_equals(const String* str1, const String* str2) {
    if (!str1 || !str2 || str1->len != str2->len)
        return false;

    return strcmp(string_cstr(str1), string_cstr(str2)) ? false : true;
}

boolean string_equals_cstr(const String* str, const char* cstr) {
    if (!str || !cstr || str->len != strlen(cstr))
        return false;

    return strcmp(string_cstr(str), cstr) ? false : true;
}

void string_free(String* c_pstr) {
    tfree(c_pstr->ptr);
    tfree(c_pstr);
}

string_itr* string_get_itr(const String* c_str) {
    string_itr* str_itr = tmalloc(sizeof(string_itr));
    if (!str_itr)
        return NULL;

    str_itr->start = c_str->ptr;
    str_itr->cur = c_str->ptr;
    str_itr->size = c_str->len + 1;

    return str_itr;
}

u8 string_itr_next(string_itr* str_itr) {
    return (str_itr->cur - str_itr->start) < str_itr->size ? *(str_itr->cur++) : *str_itr->cur;
}

void string_itr_free(string_itr* str_itr) {
    tfree(str_itr);
}

string_slice* string_get_slice(const String* str, const usize s, const usize e) {
    if (!str || s < 0 || s >= str->len || e < s || e >= str->len)
        return NULL;

    string_slice* str_sl = tmalloc(sizeof(string_slice));
    if (!str_sl)
        return NULL;

    str_sl->s = tmalloc(e - s + 1);
    if (!str_sl) {
        tfree(str_sl);
        return NULL;
    }

    str_sl->len = e - s + 1;
    memcpy(str_sl->s, str->ptr + s, e - s + 1);

    return str_sl;
}

String* string_from_slice(const string_slice* str_sl) {
    if (!str_sl)
        return NULL;

    String* str = tmalloc(sizeof(String));
    if (!str)
        return NULL;

    str->ptr = tmalloc(str_sl->len + 1);
    if (!str->ptr) {
        tfree(str);
        return NULL;
    }

    memcpy(str->ptr, str_sl->s, str_sl->len);
    str->len = str_sl->len;
    *(str->ptr + str->len) = '\0';
    return str;
}

boolean string_equals_slice(string_slice* str_sl_1, string_slice* str_sl_2) {
    if (!str_sl_1 || !str_sl_2 || str_sl_1->len != str_sl_2->len)
        return false;

    return strcmp((char*) str_sl_1->s, (char*) str_sl_2->s) ? false : true;
}

string_slice* string_set_slice(string_slice* self, const string_slice* str_sl) {
    u8* tmp_ptr = trealloc(self->s, str_sl->len);
    if (!tmp_ptr)
        return NULL;

    self->s = tmp_ptr;
    self->len = str_sl->len;
    memcpy(self->s, str_sl->s, str_sl->len);
    return self;
}

String* string_push_back_slice(String* dest, const string_slice* src) {
    if (!dest || !src)
        return NULL;

    u8* tmp_ptr = trealloc(dest->ptr, dest->len + src->len + 1);
    if (!tmp_ptr)
        return NULL;

    dest->ptr = tmp_ptr;
    memcpy(dest->ptr + dest->len, src->s, src->len);
    dest->len += src->len;

    return dest;
}

String* string_push_front_slice(String* dest, const string_slice* src) {
    if (!dest || !src)
        return NULL;

    u8* tmp_ptr = trealloc(dest->ptr, dest->len + src->len + 1);
    if (!tmp_ptr)
        return NULL;

    dest->ptr = tmp_ptr;
    memcpy(dest->ptr + src->len, dest->ptr, dest->len);
    memcpy(dest->ptr, src->s, src->len);
    dest->len += src->len;

    return dest;
}

String* string_insert_slice_ex(String* dest, const string_slice* src, const usize i) {
    if (i == 0)
        return string_push_front_slice(dest, src);

    u8* tmp_ptr = trealloc(dest->ptr, dest->len + src->len + 1);
    if (!tmp_ptr)
        return NULL;

    dest->ptr = tmp_ptr;
    memcpy(dest->ptr + src->len + i, dest->ptr + i, dest->len - i + 1);
    memcpy(dest->ptr + i, src->s, src->len);
    dest->len += src->len;

    return dest;
}

String* string_insert_slice_by_byte(String* dest, const string_slice* src, const usize b) {
    usize len = string_len(dest);
    if (len == -1 || b < 0 || b > len || !src)
        return NULL;

    return string_insert_slice_ex(dest, src, b);
}


String* string_insert_slice(String* dest, const string_slice* src, const usize i) {
    usize len = string_len(dest);
    if (len == -1 || i < 0 || i >= len || !src)
        return NULL;

    return string_insert_slice_ex(dest, src, i);
}

void string_slice_free(string_slice* str_sl) {
    tfree(str_sl);
}

//<--------------------------- UTF-8 --------------------------->

Error string_utf8_new(string_utf8** str) {
    ERROR_ARG_CHECK(str, { return ERROR_INVALID_ARGUMENT; });

    string_utf8* str_new = tmalloc(sizeof(string_utf8));
    ERROR_ARG_CHECK(str_new, { return ERROR_INVALID_ARGUMENT; });

    str_new->ptr = tmalloc(1);
    ERROR_ARG_CHECK(str_new->ptr, { return ERROR_INVALID_ARGUMENT; });

    *(u8*) str_new->ptr = '\0';
    str_new->len = 0;
    *str = str_new;

    return ERROR_SUCCESS;
}

static Error string_utf8_dec(string_utf8** str, const u8* c_str) {
    usize len_c_cstr = strlen((char*) c_str);
    if (len_c_cstr == 0) {
        return string_utf8_new(str);
    }

    u32 cache[4 * len_c_cstr + 1];
    u32* tmp_ptr = cache;

    while (*c_str != 0) {
        if ((*c_str & 0x80) == 0) {
            *(tmp_ptr++) = *(c_str++);
        } else if ((c_str[0] & 0xE0) == 0xC0 && (c_str[1] & 0xC0) == 0x80) {
            memcpy(tmp_ptr++, c_str, 2);
            c_str += 2;
        } else if ((c_str[0] & 0xF0) == 0xE0 && (c_str[1] & 0xC0) == 0x80 && (c_str[2] & 0xC0) == 0x80) {
            memcpy(tmp_ptr++, c_str, 3);
            c_str += 3;
        } else if ((c_str[0] & 0xF8) == 0xF0 && (c_str[1] & 0xC0) == 0x80 && (c_str[2] & 0xC0) == 0x80 &&
                   (c_str[3] & 0xC0) == 0x80) {
            memcpy(tmp_ptr++, c_str, 4);
            c_str += 4;
        } else {
            LOG_ERROR("Invalid argument %s: Incorrect utf8 format", c_str);
            return ERROR_INVALID_ARGUMENT;
        }
    }
    *(u8*) tmp_ptr = '\0';

    string_utf8* str_new = tmalloc(sizeof(string_utf8));
    ERROR_ARG_CHECK(str_new, { return ERROR_INVALID_ARGUMENT; });

    usize size_new_str = 4 * (tmp_ptr - cache) + 1;
    str_new->ptr = tmalloc(size_new_str);
    ERROR_ARG_CHECK(str_new->ptr, { return ERROR_INVALID_ARGUMENT; });
    str_new->len = tmp_ptr - cache;

    memcpy(str_new->ptr, cache, size_new_str);
    *str = str_new;

    return ERROR_SUCCESS;
}

Error string_utf8_to_string(String** dest, const string_utf8* str) {
    ERROR_ARGS_CHECK_2(dest, str, { return ERROR_INVALID_ARGUMENT; });

    usize len_str;
    string_utf8_len(&len_str, str);
    if (len_str == 0)
        return string_new(dest);

    usize size_str;
    string_utf8_size(&size_str, str);

    u8 cache[size_str];
    u8* tmp_ptr = cache;

    for (usize i = 0; i < len_str; i++) {
        u8 c = *(u8*) (str->ptr + i);

        u8 b7 = (c & 0x80) >> 7;
        u8 b6 = (c & 0x40) >> 6;
        u8 b5 = (c & 0x20) >> 5;
        u8 b4 = (c & 0x10) >> 4;

        u8 cnt = 1 + b7 * (b6 + b6 * (b5 + b5 * b4));

        memcpy(tmp_ptr, str->ptr + i, cnt);
        tmp_ptr += cnt;
    }
    *tmp_ptr = '\0';

    String* str_new = tmalloc(sizeof(String));
    ERROR_ARG_CHECK(str_new, { return ERROR_INVALID_ARGUMENT; });

    usize size_new_str = tmp_ptr - cache + 1;
    str_new->ptr = tmalloc(size_new_str);
    ERROR_ARG_CHECK(str_new->ptr, { return ERROR_INVALID_ARGUMENT; });
    str_new->len = size_new_str - 1;

    memcpy(str_new->ptr, cache, size_new_str);
    *dest = str_new;

    return ERROR_SUCCESS;
}

Error string_utf8_from(string_utf8** str, const char* c_str) {
    ERROR_ARGS_CHECK_2(str, c_str, { return ERROR_INVALID_ARGUMENT; });
    return string_utf8_dec(str, (u8*) c_str);
}


Error string_utf8_len(usize* len, const string_utf8* c_str) {
    ERROR_ARGS_CHECK_2(len, c_str, { return ERROR_INVALID_ARGUMENT; });
    *len = c_str->len;
    return ERROR_SUCCESS;
}

Error string_utf8_size(usize* size, const string_utf8* c_str) {
    ERROR_ARGS_CHECK_2(size, c_str, { return ERROR_INVALID_ARGUMENT; });
    *size = 4 * c_str->len + 1;
    return ERROR_SUCCESS;
}

Error string_utf8_clone(string_utf8** str, const string_utf8* c_str) {
    ERROR_ARGS_CHECK_2(str, c_str, { return ERROR_INVALID_ARGUMENT; });

    string_utf8* str_new = tmalloc(sizeof(String));
    ERROR_ARG_CHECK(str_new, { return ERROR_INVALID_ARGUMENT; });

    usize size_c_str = 4 * c_str->len + 1;
    str_new->ptr = tmalloc(size_c_str);
    ERROR_ARG_CHECK(str_new->ptr, { return ERROR_INVALID_ARGUMENT; });
    str_new->len = c_str->len;
    memcpy(str_new->ptr, c_str->ptr, size_c_str);
    *str = str_new;

    return ERROR_SUCCESS;
}

Error string_utf8_push_back_cstr(string_utf8* dest, const char* src) {
    ERROR_ARGS_CHECK_2(dest, src, { return ERROR_INVALID_ARGUMENT; });

    string_utf8* str_new;
    if (string_utf8_dec(&str_new, (u8*) src)) {
        LOG_ERROR("Invalid argument: error dec");
        return ERROR_INVALID_ARGUMENT;
    }

    if (string_utf8_push_back(dest, str_new)) {
        LOG_ERROR("Invalid argument: error push_back");
        return ERROR_INVALID_ARGUMENT;
    }

    string_utf8_free(str_new);
    return ERROR_SUCCESS;
}

Error string_utf8_push_back(string_utf8* dest, const string_utf8* src) {
    ERROR_ARGS_CHECK_2(dest, src, { return ERROR_INVALID_ARGUMENT; });

    usize size_dest = 4 * dest->len + 1;
    usize size_src = 4 * src->len + 1;
    u32* tmp_ptr = trealloc(dest->ptr, size_dest + size_src - 1);
    ERROR_ARG_CHECK(tmp_ptr, { return ERROR_INVALID_ARGUMENT; });

    dest->ptr = tmp_ptr;
    memcpy(dest->ptr + dest->len, src->ptr, size_src);
    dest->len += src->len;

    return ERROR_SUCCESS;
}

Error string_utf8_push_front(string_utf8* dest, const string_utf8* src) {
    ERROR_ARGS_CHECK_2(dest, src, { return ERROR_INVALID_ARGUMENT; });

    usize size_dest = 4 * dest->len + 1;
    usize size_src = 4 * src->len + 1;
    u32* tmp_ptr = trealloc(dest->ptr, size_dest + size_src - 1);
    ERROR_ARG_CHECK(tmp_ptr, { return ERROR_INVALID_ARGUMENT; });

    dest->ptr = tmp_ptr;
    memcpy(dest->ptr + src->len, dest->ptr, size_dest);
    memcpy(dest->ptr, src->ptr, size_src - 1);
    dest->len += src->len;

    return ERROR_SUCCESS;
}

Error string_utf8_insert(string_utf8* dest, const string_utf8* src, const usize i) {
    ERROR_ARGS_CHECK_2(dest, src, { return ERROR_INVALID_ARGUMENT; });

    if (i == 0)
        return string_utf8_push_front(dest, src);

    usize size_dest = 4 * dest->len + 1;
    usize size_src = 4 * src->len + 1;
    u32* tmp_ptr = trealloc(dest->ptr, size_dest + size_src - 1);
    ERROR_ARG_CHECK(tmp_ptr, { return ERROR_INVALID_ARGUMENT; });

    dest->ptr = tmp_ptr;
    memcpy(dest->ptr + src->len + i, dest->ptr + i, 4 * (dest->len - i + 1));
    memcpy(dest->ptr + i, src->ptr, size_src - 1);
    dest->len += src->len;

    return ERROR_SUCCESS;
}

void string_utf8_free(string_utf8* c_pstr) {
    tfree(c_pstr->ptr);
    tfree(c_pstr);
}
