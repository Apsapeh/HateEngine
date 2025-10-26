#pragma once

#include <types/types.h>

/**
 * @brief Primitive 4D vector
 *
 * @api forward
 */
typedef struct {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
} UVec4;

// MACROS API START
#define UVEC4_NEW_M(v_x, v_y, v_z, v_w)                                                                 \
    (UVec4) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z, .w = v_w                                                          \
    }
#define UVEC4_NEW_VAL_M(v) UVEC4_NEW_M(v, v, v, v)
#define UVEC4_ZERO_M UVEC4_NEW_VAL_M(0);
#define UVEC4_ONE_M UVEC4_NEW_VAL_M(1);
#define UVEC4_MAX_M UVEC4_NEW_VAL_M(U32_MAX);
// #define UVEC4_MIN_M (UVec4){.x=I32_MIN, .y=I32_MIN}
// MACROS API END

/**
 * @brief
 *
 * @api
 */
void uvec4_init(u32 x, u32 y, u32 z, u32 w, UVec4* const out);

/**
 * @brief
 *
 * @api
 */
void uvec4_add(const UVec4* const a, const UVec4* const b, UVec4* const out);

/**
 * @brief
 *
 * @api
 */
void uvec4_sub(const UVec4* const a, const UVec4* const b, UVec4* const out);

/**
 * @brief
 *
 * @api
 */
void uvec4_scale(const UVec4* const a, const float factor, UVec4* const out);

/**
 * @brief
 *
 * @api
 */
float uvec4_dot(const UVec4* const a, const UVec4* const b);

/**
 * @brief
 *
 * @api
 */
float uvec4_length(const UVec4* const a);

/**
 * @brief
 *
 * @api
 */
void uvec4_normalize(const UVec4* const a, UVec4* const out);

/**
 * @brief
 *
 * @api
 */
float uvec4_distance(const UVec4* const a, const UVec4* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
void uvec4_add_in(UVec4* const to, const UVec4* const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
void uvec4_sub_in(UVec4* const from, const UVec4* const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
void uvec4_scale_in(UVec4* const to, const float factor);

// void uvec4_cross_in(UVec4* const to, const UVec4 *const b);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
void uvec4_normalize_in(UVec4* const a);
