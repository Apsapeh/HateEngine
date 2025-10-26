#pragma once

#include <types/types.h>

/**
 * @brief Primitive 3D vector
 *
 * @api forward
 */
typedef struct {
    i32 x;
    i32 y;
    i32 z;
} IVec3;

// MACROS API START
#define IVEC3_NEW_M(v_x, v_y, v_z)                                                                      \
    (IVec3) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z                                                                    \
    }
#define IVEC3_NEW_VAL_M(v) IVEC3_NEW_M(v, v, v)
#define IVEC3_ZERO_M IVEC3_NEW_VAL_M(0)
#define IVEC3_ONE_M IVEC3_NEW_VAL_M(1)
#define IVEC3_MINUS_ONE_M IVEC3_NEW_VAL_M(-1)
#define IVEC3_MAX_M IVEC3_NEW_VAL_M(I32_MAX)
#define IVEC3_MIN_M IVEC3_NEW_VAL_M(I32_MIN)
// MACROS API END

/**
 * @brief
 *
 * @api
 */
void ivec3_init(i32 x, i32 y, i32 z, IVec3* const out);

/**
 * @brief
 *
 * @api
 */
void ivec3_add(const IVec3* const a, const IVec3* const b, IVec3* const out);

/**
 * @brief
 *
 * @api
 */
void ivec3_sub(const IVec3* const a, const IVec3* const b, IVec3* const out);

/**
 * @brief
 *
 * @api
 */
void ivec3_scale(const IVec3* const a, const float factor, IVec3* const out);

/**
 * @brief
 *
 * @api
 */
void ivec3_cross(const IVec3* const a, const IVec3* const b, IVec3* const out);

/**
 * @brief
 *
 * @api
 */
float ivec3_dot(const IVec3* const a, const IVec3* const b);

/**
 * @brief
 *
 * @api
 */
float ivec3_length(const IVec3* const a);

/**
 * @brief
 *
 * @api
 */
void ivec3_normalize(const IVec3* const a, IVec3* const out);

/**
 * @brief
 *
 * @api
 */
float ivec3_distance(const IVec3* const a, const IVec3* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
void ivec3_add_in(IVec3* const to, const IVec3* const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
void ivec3_sub_in(IVec3* const from, const IVec3* const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
void ivec3_scale_in(IVec3* const to, const float factor);

// void ivec3_cross_in(IVec3* const to, const IVec3 *const b);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
void ivec3_normalize_in(IVec3* const a);
