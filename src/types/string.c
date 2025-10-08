#include "string.h"

string string_new(void) {
    string res;
    res.cstr_vec = vec_char_init();

    if (!vec_char_push_back(&res.cstr_vec, '\0'))
        LOG_FATAL("КОМП ГОВНОООО")

    res.str = res.cstr_vec.data;
    res.size = 0;
    return res;
}

string string_from(const char* str) {
    return string_from_n(str, strlen(str));
}

string string_from_n(const char* str, unsigned long n) {
    string res;
    res.cstr_vec = vec_char_init();

    if (!vec_char_append_data(&res.cstr_vec, (char*) str, n))
        LOG_FATAL("АЛЛАХУ АКБАРРРРРРРРРР");

    if (res.cstr_vec.data[res.cstr_vec.size - 1] != '\0')
        if (!vec_char_push_back(&res.cstr_vec, '\0'))
            LOG_FATAL("АЛЛАХУ АКБАРРРРРРРРРР");

    res.size = res.cstr_vec.size - 1;
    res.str = res.cstr_vec.data;
    return res;
}

unsigned char string_insert(string* self, unsigned long index, char c) {
    if (index > self->size) return 0;
    if (!vec_char_insert(&self->cstr_vec, index, c))
        LOG_FATAL("ОТДОХНИ");

    self->size = self->cstr_vec.size - 1;
    return 1;
}

unsigned char string_insert_str(string* self, unsigned long index, const char* str) {
    if (index > self->size) return 0;
    if (!vec_char_insert_data(&self->cstr_vec, index, (char*) str, strlen(str)))
        LOG_FATAL("ОТДОХНИ");

    self->size = self->cstr_vec.size - 1;
    return 1;
}
    

void string_replace(string* str, const char* pat, const char* rep) {
    unsigned long pat_len = strlen(pat);
    unsigned long rep_len = strlen(rep);

    for (unsigned long i = 0; i < str->size; ++i) {
        if (strncmp(str->str + i, pat, pat_len) == 0) {
            if (!string_erase_range(str, i, pat_len))
                LOG_FATAL("ОТДОХНИ");
            
            if (!string_insert_str(str, i, rep))
                LOG_FATAL("ОТДОХНИ");

            i += rep_len-1;
        }
    }
}

unsigned char string_erase(string* self, unsigned long index) {
    if (index >= self->size) return 0;
    if (!vec_char_erase(&self->cstr_vec, index))
        LOG_FATAL("ОТДОХНИ");

    self->size = self->cstr_vec.size - 1;
    return 1;
}
    
unsigned char string_erase_range(string *self, unsigned long index, unsigned long count) {
    if (index + count > self->size) return 0;
    if (!vec_char_erase_range(&self->cstr_vec, index, count))
        LOG_FATAL("ОТДОХНИ");

    self->size = self->cstr_vec.size - 1;
    return 1;
}

void string_strip(string* str) {
    for (char* ch = str->str; ch < (str->str + str->cstr_vec.size); ++ch)
        if (*ch != ' ' && *ch != '\t' && *ch != '\n') {
            string_erase_range(str, 0, ch - str->str);
            break;
        }
    
    for (char* ch = str->str + str->size; ch - str->str > 0; --ch)
        if (*ch != ' ' && *ch != '\t' && *ch != '\n') {
            string_erase_range(str, ch - str->str, str->size - (ch - str->str));
            break;
        }
}
    

void string_free(string* self) {\
    vec_char_free(&self->cstr_vec);
    self->size = 0;
    self->str = NULL;
}

void vec_string_free(vec_string* vec) {
    for (unsigned long i = 0; i < vec->size; ++i)
        string_free(&vec->data[i]);

    vec_string_free(vec);
}
