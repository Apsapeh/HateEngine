#pragma once

#include <types/types.h>

/**
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct {
    u32 x;
    u32 y;
} UVec2;

// MACROS API START
#define UVEC2_NEW_M(v_x, v_y)                                                                           \
    (UVec2) {                                                                                           \
        .x = v_x, .y = v_y                                                                              \
    }
#define UVEC2_NEW_VAL_M UVEC2_NEW_M(v, v)
#define UVEC2_ZERO_M UVEC2_NEW_VAL_M(0)
#define UVEC2_ONE_M UVEC2_NEW_VAL_M(1)
#define UVEC2_MAX_M UVEC2_NEW_VAL_M(I32_MAX)
// MACROS API END

/**
 * @api
 */
void uvec2_init(const u32 x, const u32 y, UVec2* const out);

/**
 * @api
 */
void uvec2_add(const UVec2* const a, const UVec2* const b, UVec2* const out);

/**
 * @api
 */
void uvec2_sub(const UVec2* const a, const UVec2* const b, UVec2* const out);

/**
 * @api
 */
void uvec2_scale(const UVec2* const a, const float factor, UVec2* const out);

/**
 * @api
 */
float uvec2_dot(const UVec2* const a, const UVec2* const b);

/**
 * @api
 */
float uvec2_length(const UVec2* const a);

/**
 * @api
 */
void uvec2_normalize(const UVec2* const a, UVec2* const out);

/**
 * @api
 */
float uvec2_distance(const UVec2* const a, const UVec2* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
void uvec2_add_in(UVec2* const to, const UVec2* const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
void uvec2_sub_in(UVec2* const from, const UVec2* const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
void uvec2_scale_in(UVec2* const to, const float factor);

// void uvec2_cross_in(UVec2* const to, const UVec2 *const b);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
void uvec2_normalize_in(UVec2* const a);
