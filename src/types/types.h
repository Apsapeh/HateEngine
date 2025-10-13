#pragma once

#include <stddef.h>
#include <stdint.h>

// API START
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t usize;

typedef float f32;
typedef double f64;

typedef uintptr_t uptr;
typedef intptr_t iptr;

typedef u8 boolean;
#define true 1
#define false 0

typedef char* str;
typedef const char* c_str;

typedef void (*fptr)(void);
typedef void* ptr;
