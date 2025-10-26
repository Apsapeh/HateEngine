#pragma once

#include <types/types.h>

/**
 * @brief Primitive 4D vector
 *
 * @api forward
 */
typedef struct {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
} IVec4;

// MACROS API START
#define IVEC4_NEW_M(v_x, v_y, v_z, v_w)                                                                 \
    (IVec4) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z, .w = v_w                                                          \
    }
#define IVEC4_NEW_VAL_M(v) IVEC4_NEW_M(v, v, v, v)
#define IVEC4_ZERO_M IVEC4_NEW_VAL_M(0);
#define IVEC4_ONE_M IVEC4_NEW_VAL_M(1);
#define IVEC4_MINUS_ONE_M IVEC4_NEW_VAL_M(-1);
#define IVEC4_MAX_M IVEC4_NEW_VAL_M(I32_MAX);
#define IVEC4_MIN_M IVEC4_NEW_VAL_M(I32_MIN);
// #define IVEC4_MIN_M (IVec4){.x=I32_MIN, .y=I32_MIN}
// MACROS API END

/**
 * @brief
 *
 * @api
 */
void ivec4_init(i32 x, i32 y, i32 z, i32 w, IVec4* const out);

/**
 * @brief
 *
 * @api
 */
void ivec4_add(const IVec4* const a, const IVec4* const b, IVec4* const out);

/**
 * @brief
 *
 * @api
 */
void ivec4_sub(const IVec4* const a, const IVec4* const b, IVec4* const out);

/**
 * @brief
 *
 * @api
 */
void ivec4_scale(const IVec4* const a, const float factor, IVec4* const out);

/**
 * @brief
 *
 * @api
 */
float ivec4_dot(const IVec4* const a, const IVec4* const b);

/**
 * @brief
 *
 * @api
 */
float ivec4_length(const IVec4* const a);

/**
 * @brief
 *
 * @api
 */
void ivec4_normalize(const IVec4* const a, IVec4* const out);

/**
 * @brief
 *
 * @api
 */
float ivec4_distance(const IVec4* const a, const IVec4* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
void ivec4_add_in(IVec4* const to, const IVec4* const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
void ivec4_sub_in(IVec4* const from, const IVec4* const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
void ivec4_scale_in(IVec4* const to, const float factor);

// void ivec4_cross_in(IVec4* const to, const IVec4 *const b);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
void ivec4_normalize_in(IVec4* const a);
