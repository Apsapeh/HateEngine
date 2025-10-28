#include "string.h"
#include <string.h>
#include "../error.h"
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

    return string_insert_cstr_ex(self, src, i, strlen(src));
}

String* string_insert_cstr_by_byte(String* self, const char* src, const usize b) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (b < 0 || b > len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): b");
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
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): b");
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
        LOG_ERROR_OR_DEBUG_FATAL("Invalid arguments (output for boundary): i or n");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    u8* tmp_ptr;

    if (i + n == self->len - 1) { // если мы удаляем до конца строки начиная с i-индекса
        tmp_ptr =
                trealloc(self->ptr, i + 1); // в этом случае просто оставляем место для i кол-во символов
        ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });
        *(tmp_ptr + i) = '\0';
    } else {
        usize len_cache = self->len - i - n;
        u8* cache = tmalloc(len_cache);
        ERROR_ALLOC_CHECK(cache, { return NULL; });

        memcpy(cache, self->ptr + i + n + 1, len_cache);
        tmp_ptr = trealloc(self->ptr, i + len_cache);
        ERROR_ALLOC_CHECK(tmp_ptr, {
            tfree(cache);
            return NULL;
        });

        memcpy(tmp_ptr + i, cache, len_cache);
        tfree(cache);
    }

    self->ptr = tmp_ptr;
    self->len -= n + 1;

    return self;
}

String* string_remove_n_by_byte(String* self, const usize b, const usize b_n) {
    return string_remove_n(self, b, b_n);
}

boolean string_equals(const String* str1, const String* str2) {
    ERROR_ARGS_CHECK_2(str1, str2, { return false; });
    if (str1->len != str2->len)
        return false;

    return strcmp(string_cstr(str1), string_cstr(str2)) ? false : true;
}

boolean string_equals_cstr(const String* str, const char* c_str) {
    ERROR_ARGS_CHECK_2(str, c_str, { return false; });
    if (str->len != strlen(c_str))
        return false;

    return strcmp(string_cstr(str), c_str) ? false : true;
}

void string_free(String* self) {
    tfree(self->ptr);
    tfree(self);
}


StringSlice* string_get_slice(const String* self, const usize s, const usize e) {
    ERROR_ARG_CHECK(self, { return NULL; });

    if (s < 0 || s >= self->len || e < s || e >= self->len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): s or e");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    StringSlice* str_sl = tmalloc(sizeof(StringSlice));
    ERROR_ALLOC_CHECK(str_sl, { return NULL; });

    str_sl->s = tmalloc(e - s + 1);
    ERROR_ALLOC_CHECK(str_sl->s, {
        tfree(str_sl);
        return NULL;
    });

    str_sl->len = e - s + 1;
    memcpy(str_sl->s, self->ptr + s, e - s + 1);

    return str_sl;
}

String* string_from_slice(const StringSlice* self) {
    ERROR_ALLOC_CHECK(self, { return NULL; });

    String* str = tmalloc(sizeof(String));
    ERROR_ALLOC_CHECK(str, { return NULL; });

    str->ptr = tmalloc(self->len + 1);
    ERROR_ALLOC_CHECK(str->ptr, {
        tfree(str);
        return NULL;
    });

    memcpy(str->ptr, self->s, self->len);
    str->len = self->len;
    *(str->ptr + str->len) = '\0';

    return str;
}

boolean string_equals_slice(StringSlice* str_sl_1, StringSlice* str_sl_2) {
    ERROR_ARGS_CHECK_2(str_sl_1, str_sl_2, { return false; });
    if (str_sl_1->len != str_sl_2->len)
        return false;

    return strcmp((char*) str_sl_1->s, (char*) str_sl_2->s) ? false : true;
}

StringSlice* string_set_slice(StringSlice* self, const StringSlice* str_sl) {
    ERROR_ARGS_CHECK_2(self, str_sl, { return NULL; });
    u8* tmp_ptr = trealloc(self->s, str_sl->len);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->s = tmp_ptr;
    self->len = str_sl->len;
    memcpy(self->s, str_sl->s, str_sl->len);

    return self;
}

String* string_push_back_slice(String* self, const StringSlice* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    u8* tmp_ptr = trealloc(self->ptr, self->len + src->len + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + self->len, src->s, src->len);
    self->len += src->len;

    return self;
}

String* string_push_front_slice(String* self, const StringSlice* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    u8* tmp_ptr = trealloc(self->ptr, self->len + src->len + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + src->len, self->ptr, self->len);
    memcpy(self->ptr, src->s, src->len);
    self->len += src->len;

    return self;
}

String* string_insert_slice_ex(String* self, const StringSlice* src, const usize i) {
    if (i == 0)
        return string_push_front_slice(self, src);

    u8* tmp_ptr = trealloc(self->ptr, self->len + src->len + 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + src->len + i, self->ptr + i, self->len - i + 1);
    memcpy(self->ptr + i, src->s, src->len);
    self->len += src->len;

    return self;
}

String* string_insert_slice_by_byte(String* self, const StringSlice* src, const usize b) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (b < 0 || b > len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): b");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    return string_insert_slice_ex(self, src, b);
}


String* string_insert_slice(String* self, const StringSlice* src, const usize i) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize len = string_len(self);
    if (i < 0 || i >= len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    return string_insert_slice_ex(self, src, i);
}

void string_slice_free(StringSlice* self) {
    tfree(self->s);
    tfree(self);
}

//<--------------------------- UTF-8 --------------------------->

StringUTF8* string_utf8_new(void) {
    StringUTF8* str_new = tmalloc(sizeof(StringUTF8));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    str_new->ptr = tmalloc(1);
    ERROR_ALLOC_CHECK(str_new->ptr, { return NULL; });

    *(u8*) str_new->ptr = '\0';
    str_new->len = 0;

    return str_new;
}


static StringUTF8* string_utf8_dec(const u8* c_str, const usize len_c_str) {
    if (len_c_str == 0) {
        return string_utf8_new();
    }

    u32* cache = tmalloc(4 * len_c_str + 1);
    ERROR_ALLOC_CHECK(cache, { return NULL; });

    StringUTF8* str_new = tmalloc(sizeof(StringUTF8));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    u32* tmp_ptr = cache;

    while (*c_str != 0) { // итерируемся до '\0'
        if ((*c_str & 0x80) == 0) { // маска для 1 байта
            *(tmp_ptr) = *(c_str++);
            memset((u8*) (tmp_ptr++) + 1, 0, 3);
        } else if ((c_str[0] & 0xE0) == 0xC0 && (c_str[1] & 0xC0) == 0x80) { // маска для 2 байт
            memcpy(tmp_ptr, c_str, 2);
            memset((u8*) (tmp_ptr++) + 2, 0, 2);
            c_str += 2; // это сдвиг на кол-во добавляемых байтов
        } else if ((c_str[0] & 0xF0) == 0xE0 && (c_str[1] & 0xC0) == 0x80 &&
                   (c_str[2] & 0xC0) == 0x80) { // маска для 3 байт
            memcpy(tmp_ptr, c_str, 3);
            memset((u8*) (tmp_ptr++) + 3, 0, 1);
            c_str += 3;
        } else if ((c_str[0] & 0xF8) == 0xF0 && (c_str[1] & 0xC0) == 0x80 && (c_str[2] & 0xC0) == 0x80 &&
                   (c_str[3] & 0xC0) == 0x80) { // маска для 4 байт
            memcpy(tmp_ptr++, c_str, 4);
            c_str += 4;
        } else {
            LOG_ERROR_OR_DEBUG_FATAL("Invalid argument 'c_str': Incorrect UTF8-format");
            set_error(ERROR_INVALID_ARGUMENT);
            tfree(cache);
            tfree(str_new);
            return NULL;
        }
    }
    *(u8*) tmp_ptr = '\0'; // не забываем про '\0', он не был добавлен так как мы до него итерировались

    usize size_new_str = 4 * (tmp_ptr - cache) + 1; // размер получившейся строки в байтах
    str_new->ptr = tmalloc(size_new_str);
    ERROR_ALLOC_CHECK(str_new->ptr, {
        tfree(cache);
        tfree(str_new);
        return NULL;
    });

    str_new->len = tmp_ptr - cache;
    memcpy(str_new->ptr, cache, size_new_str); // копируем из 'cache' добавленную инфу
    tfree(cache);

    return str_new;
}

StringUTF8* string_utf8_from(const char* c_str) {
    ERROR_ARG_CHECK(c_str, { return NULL; });
    return string_utf8_dec((u8*) c_str, strlen(c_str));
}

String* string_utf8_to_string(const StringUTF8* self) {
    ERROR_ARG_CHECK(self, { return NULL; });

    usize len_str = self->len;
    if (len_str == 0) // если кол-во элементов равно 0, то можно просто вызвать 'string_new'
        return string_new();

    usize size_str = 4 * self->len + 1;

    u8* cache = tmalloc(size_str); // тут я очень упрощенно создаю кеш, взяв худшую ситуацию, что у нас
                                   // элементы все по 4 байта (без лишних)
    ERROR_ALLOC_CHECK(cache, { return NULL; });

    u8* tmp_ptr = cache;

    String* str_new = tmalloc(sizeof(String));
    ERROR_ALLOC_CHECK(str_new, {
        tfree(cache);
        return NULL;
    });

    for (usize i = 0; i < len_str; i++) {
        u8 c = *(u8*) (self->ptr + i); // получаем байт в текущей итерации

        /*
            у нас есть 4 ситуации, когда символ требует 1, 2, 3, 4 байта;
            чтобы итерироваться быстрее, будем брать старший байт и по нему определять кол-во байтов для
           символа; так как в 'string_decode' я использую memcpy для вставки то меня не волнует вопрос о
           little или big endian, потому что это гарантиурет что старший байт будет первым; остается
           только научиться определять кол-во символов по старшему байту, для этого надо почитать по ним
           доку, тогда все станет ясно пример (кол-во единиц говорит о кол-ве байт): [0]100 0101 -> 1
           byte [110]1 0101 -> 2 byte [1110] 0101 -> 3 byte [11110] 101 -> 4 byte

            также подчеркну что последующие байты после старшего (кроме символа равному 1 байту) будут
           иметь маску [10]** **** можно было бы исползовать условия, но мне кажется это медленнее чем
           расчетный способ
        */
        u8 b7 = (c & 0x80) >> 7; // получение 7 бита (отсчет с нуля)
        u8 b6 = (c & 0x40) >> 6; // получение 6 бита
        u8 b5 = (c & 0x20) >> 5; // получение 5 бита
        u8 b4 = (c & 0x10) >> 4; // получение 4 бита

        /*
            cnt (count) - кол-во байт в числе

            если мы просто сложим биты то это может привести к неправильному определению
            пример:

            7654 3210 (индексы)
            [0101] 0000

            как видно у нас символ должен хранить байт, но неправильный способ скажет что у нас 2 байта
            я решил считать биты не просто так, а именно слева на право
            в случае с верхним примером мы поступим так:
            1) добавим b7 (чтобы данные правильно интерпретировались, надо вместо него сложить 1)
            2) затем будем добавлять другие биты, но умножим все их на b7, это приводит к тому, что если
           7 бит равен 0,то он просто обнулит остальные
        */
        u8 cnt = 1 + b7 * (b6 + b6 * (b5 + b5 * b4));

        memcpy(tmp_ptr, self->ptr + i, cnt);
        tmp_ptr += cnt;
    }
    *tmp_ptr = '\0';

    usize size_new_str = tmp_ptr - cache + 1;
    str_new->ptr = tmalloc(size_new_str);
    ERROR_ALLOC_CHECK(str_new, {
        tfree(cache);
        tfree(str_new);
        return NULL;
    });

    str_new->len = size_new_str - 1;
    memcpy(str_new->ptr, cache, size_new_str);

    tfree(cache);
    return str_new;
}


usize string_utf8_len(const StringUTF8* self) {
    return self->len;
}

usize string_utf8_size(const StringUTF8* self) {
    return 4 * self->len + 1;
}


StringUTF8* string_utf8_clone(const StringUTF8* self) {
    ERROR_ARG_CHECK(self, { return NULL; });

    StringUTF8* str_new = tmalloc(sizeof(String));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    usize size_self = string_utf8_size(self);
    str_new->ptr = tmalloc(size_self);
    ERROR_ALLOC_CHECK(str_new->ptr, {
        tfree(str_new);
        return NULL;
    });

    str_new->len = self->len;
    memcpy(str_new->ptr, self->ptr, size_self);

    return str_new;
}


StringUTF8* string_utf8_push_back_cstr(StringUTF8* self, char* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    StringUTF8* str_new = string_utf8_dec((u8*) src, strlen(src));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    StringUTF8* out = string_utf8_push_back(self, str_new);
    string_utf8_free(str_new);
    ERROR_ALLOC_CHECK(out, { return NULL; });

    return out;
}


StringUTF8* string_utf8_push_back(StringUTF8* self, const StringUTF8* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize size_dest = string_utf8_size(self);
    usize size_src = string_utf8_size(src);

    u32* tmp_ptr = trealloc(self->ptr, size_dest + size_src - 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + self->len, src->ptr, size_src);
    self->len += src->len;

    return self;
}

StringUTF8* string_utf8_insert(StringUTF8* self, const StringUTF8* src, const usize i) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    if (i == 0)
        return string_utf8_push_front(self, src);

    if (i < 0 || i >= self->len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    usize size_dest = string_utf8_size(self);
    usize size_src = string_utf8_size(src);

    u32* tmp_ptr = trealloc(self->ptr, size_dest + size_src - 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + src->len + i, self->ptr + i, 4 * (self->len - i + 1));
    memcpy(self->ptr + i, src->ptr, size_src - 1);
    self->len += src->len;

    return self;
}

StringUTF8* string_utf8_push_front(StringUTF8* self, const StringUTF8* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    usize size_dest = string_utf8_size(self);
    usize size_src = string_utf8_size(src);

    u32* tmp_ptr = trealloc(self->ptr, size_dest + size_src - 1);
    ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

    self->ptr = tmp_ptr;
    memcpy(self->ptr + src->len, self->ptr, size_dest);
    memcpy(self->ptr, src->ptr, size_src - 1);
    self->len += src->len;

    return self;
}


StringUTF8* string_utf8_push_front_cstr(StringUTF8* self, char* src) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    StringUTF8* str_new = string_utf8_dec((u8*) src, strlen(src));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    StringUTF8* out = string_utf8_push_front(self, str_new);
    string_utf8_free(str_new);
    ERROR_ALLOC_CHECK(out, { return NULL; });

    return out;
}

StringUTF8* string_utf8_insert_cstr(StringUTF8* self, const char* src, const usize i) {
    ERROR_ARGS_CHECK_2(self, src, { return NULL; });

    StringUTF8* str_new = string_utf8_dec((u8*) src, strlen(src));
    ERROR_ALLOC_CHECK(str_new, { return NULL; });

    StringUTF8* out = string_utf8_insert(self, str_new, i);
    string_utf8_free(str_new);
    ERROR_ALLOC_CHECK(out, { return NULL; });

    return out;
}

StringUTF8* string_utf8_remove(StringUTF8* self, const usize i) {
    ERROR_ARG_CHECK(self, { return NULL; });

    if (i < 0 || i >= self->len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid argument (output for boundary): i");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    if (i == (self->len - 1)) {
        u32* tmp_ptr = trealloc(self->ptr, string_utf8_size(self) - 4);
        ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

        self->ptr = tmp_ptr;
        self->len -= 1;
        *((u8*) (self->ptr + string_utf8_len(self))) = '\0';
    } else {
        u32 cache = *((u8*) (self->ptr + string_utf8_len(self)) - 3);
        u32* tmp_ptr = trealloc(self->ptr, string_utf8_size(self) - 4);
        ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });

        self->ptr = tmp_ptr;
        memcpy(self->ptr + i, self->ptr + i + 1, string_utf8_size(self) - 4 * (i + 1) - 4);
        self->len -= 1;
        *((u8*) (self->ptr + string_utf8_len(self)) - 3) = cache;
    }

    return self;
}

StringUTF8* string_utf8_remove_n(StringUTF8* self, const usize i, const usize n) {
    if (n == 0)
        return string_utf8_remove(self, i);

    ERROR_ARG_CHECK(self, { return NULL; });

    if (i < 0 || i >= self->len || n < 0 || i + n >= self->len) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid arguments (output for boundary): i or n");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    u32* tmp_ptr;

    if (i + n == self->len - 1) {
        tmp_ptr = trealloc(self->ptr, 4 * i + 1);
        ERROR_ALLOC_CHECK(tmp_ptr, { return NULL; });
        *(u8*) (tmp_ptr + i) = '\0';
    } else {
        usize size = string_utf8_size(self);
        usize len = self->len;
        usize cnt_del_el = n + 1; // кол-во удаляемых элементов
        usize aft_del_el_off = i + n + 1; // смещение на даные после удалямых элементов
        usize cnt_el_aft_del_el = len - aft_del_el_off; // кол-во элементов после удаляемых элементов
        usize end_off = len;
        usize len_cache;

        if (cnt_el_aft_del_el < cnt_del_el)
            len_cache = cnt_el_aft_del_el;
        else
            len_cache = cnt_del_el;

        u32* cache = tmalloc(4 * len_cache);
        ERROR_ALLOC_CHECK(cache, { return NULL; });

        memcpy(cache, self->ptr + end_off - len_cache, 4 * len_cache);

        tmp_ptr = trealloc(self->ptr, size - 4 * cnt_del_el);
        ERROR_ALLOC_CHECK(tmp_ptr, {
            tfree(cache);
            return NULL;
        });

        memcpy(tmp_ptr + i, tmp_ptr + aft_del_el_off, 4 * (cnt_el_aft_del_el - len_cache));
        memcpy(tmp_ptr + i + cnt_el_aft_del_el - len_cache, cache, 4 * len_cache);
        *(u8*) (tmp_ptr + end_off - len_cache) = '\0';
        tfree(cache);
    }

    self->ptr = tmp_ptr;
    self->len -= n + 1;

    return self;
}

StringUTF8* string_utf8_by_string(const String* self) {
    ERROR_ARG_CHECK(self, { return NULL; });

    return string_utf8_dec(self->ptr, self->len);
}

boolean string_utf8_equals(const StringUTF8* str1, const StringUTF8* str2) {
    ERROR_ARGS_CHECK_2(str1, str2, { return false; });

    if (str1->len != str2->len)
        return false;

    // return strcmp((char*) str1->ptr, (char*) str2->ptr) ? false : true;
    return memcmp(str1->ptr, str2->ptr, string_utf8_size(str1)) ? false : true;
}

void string_utf8_free(StringUTF8* self) {
    tfree(self->ptr);
    tfree(self);
}
