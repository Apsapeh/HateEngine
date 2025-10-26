#pragma once

#include <types/types.h>

/**
 * @brief Primitive 3D vector
 *
 * @api forward
 */
typedef struct {
    u32 x;
    u32 y;
    u32 z;
} UVec3;

// MACROS API START
#define UVEC3_NEW_M(v_x, v_y, v_z)                                                                      \
    (UVec3) {                                                                                           \
        .x = v_x, .y = v_y, .z = v_z                                                                    \
    }
#define UVEC3_NEW_VAL_M(v) UVEC3_NEW_M(v, v, v)
#define UVEC3_ZERO_M UVEC3_NEW_VAL_M(0)
#define UVEC3_ONE_M UVEC3_NEW_VAL_M(1)
#define UVEC3_MAX_M UVEC3_NEW_VAL_M(U32_MAX)
// MACROS API END

/**
 * @brief
 *
 * @api
 */
void uvec3_init(u32 x, u32 y, u32 z, UVec3* const out);

/**
 * @brief
 *
 * @api
 */
void uvec3_add(const UVec3* const a, const UVec3* const b, UVec3* const out);

/**
 * @brief
 *
 * @api
 */
void uvec3_sub(const UVec3* const a, const UVec3* const b, UVec3* const out);

/**
 * @brief
 *
 * @api
 */
void uvec3_scale(const UVec3* const a, const float factor, UVec3* const out);

/**
 * @brief
 *
 * @api
 */
void uvec3_cross(const UVec3* const a, const UVec3* const b, UVec3* const out);

/**
 * @brief
 *
 * @api
 */
float uvec3_dot(const UVec3* const a, const UVec3* const b);

/**
 * @brief
 *
 * @api
 */
float uvec3_length(const UVec3* const a);

/**
 * @brief
 *
 * @api
 */
void uvec3_normalize(const UVec3* const a, UVec3* const out);

/**
 * @brief
 *
 * @api
 */
float uvec3_distance(const UVec3* const a, const UVec3* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
void uvec3_add_in(UVec3* const to, const UVec3* const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
void uvec3_sub_in(UVec3* const from, const UVec3* const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
void uvec3_scale_in(UVec3* const to, const float factor);

// void uvec3_cross_in(UVec3* const to, const UVec3 *const b);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
void uvec3_normalize_in(UVec3* const a);
