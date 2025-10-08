#pragma once

#include <types/vector.h>
#include <string.h>
#include <math.h>
#include "../log.h"

vector_template_def(char, char)

typedef struct string {
    unsigned long size;
    char* str;
    vec_char cstr_vec;
} string;

vector_template_def(string, struct string)

string string_new(void);
string string_from(const char* str);
unsigned char string_insert(string* self, unsigned long index, char c);
unsigned char string_erase_range(string *self, unsigned long index, unsigned long count);
unsigned char string_erase(string* self, unsigned long index);
void string_replace(string* str, const char* pat, const char* rep);
static string string_from_n(const char* str, unsigned long n);
void string_free(string* self);
