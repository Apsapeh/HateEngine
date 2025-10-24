#pragma once

#include <types/types.h>

/**
 * @brief Primitive 2D vector i32
 *
 * @api forward
 */
typedef struct {
    i32 x;
    i32 y;
} IVec2;

/**
 * @api
 */
IVec2 ivec2_new(i32 x, i32 y);

/**
 * @api
 */
IVec2 ivec2_add(const IVec2* const a, const IVec2* const b);

/**
 * @api
 */
IVec2 ivec2_sub(const IVec2* const a, const IVec2* const b);

/**
 * @api
 */
IVec2 ivec2_scale(const IVec2* const a, const float factor);

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
IVec2 ivec2_normalize(const IVec2* const a);

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

// void ivec2_cross_in(Vec2* const to, const Vec2 *const b);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
void ivec2_normalize_in(IVec2* const a);
