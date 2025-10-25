#pragma once

#include <types/types.h>

/**
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct {
    i32 x;
    i32 y;
} IVec2;

// MACROS API START
#define IVEC2_NEW_M(v_x, v_y)                                                                           \
    (IVec2) {                                                                                           \
        .x = v_x, .y = v_y                                                                              \
    }
#define IVEC2_ZERO_M IVEC2_NEW_M(0, 0)
#define IVEC2_ONE_M IVEC2_NEW_M(1, 1)
#define IVEC2_MINUS_ONE_M IVEC2_NEW_M(-1, -1)
#define IVEC2_MAX_M IVEC2_NEW_M(I32_MAX, I32_MAX)
#define IVEC2_MIN_M IVEC2_NEW_M(I32_MIN, I32_MIN)
// MACROS API END

/**
 * @api
 */
void ivec2_init(const i32 x, const i32 y, IVec2* const out);

/**
 * @api
 */
void ivec2_add(const IVec2* const a, const IVec2* const b, IVec2* const out);

/**
 * @api
 */
void ivec2_sub(const IVec2* const a, const IVec2* const b, IVec2* const out);

/**
 * @api
 */
void ivec2_scale(const IVec2* const a, const float factor, IVec2* const out);

/**
 * @api
 */
float ivec2_dot(const IVec2* const a, const IVec2* const b);

/**
 * @api
 */
float ivec2_length(const IVec2* const a);

/**
 * @api
 */
void ivec2_normalize(const IVec2* const a, IVec2* const out);

/**
 * @api
 */
float ivec2_distance(const IVec2* const a, const IVec2* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
void ivec2_add_in(IVec2* const to, const IVec2* const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
void ivec2_sub_in(IVec2* const from, const IVec2* const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
void ivec2_scale_in(IVec2* const to, const float factor);

// void ivec2_cross_in(IVec2* const to, const IVec2 *const b);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
void ivec2_normalize_in(IVec2* const a);
