#pragma once

/**
 * @brief Primitive 2D vector
 *
 * @api forward
 */
typedef struct {
    float x;
    float y;
} Vec2;

/**
 * @api
 */
Vec2 vec2_new(float x, float y, float z);

/**
 * @api
 */
Vec2 vec2_add(const Vec2* const a, const Vec2* const b);

/**
 * @api
 */
Vec2 vec2_sub(const Vec2* const a, const Vec2* const b);

/**
 * @api
 */
Vec2 vec2_scale(const Vec2* const a, const float factor);

/**
 * @api
 */
float vec2_dot(const Vec2* const a, const Vec2* const b);

/**
 * @api
 */
float vec2_length(const Vec2* const a);

/**
 * @api
 */
Vec2 vec2_normalize(const Vec2* const a);

/**
 * @api
 */
float vec2_distance(const Vec2* const a, const Vec2* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param[in/out] a
 * @param[in] b
 *
 * @api
 */
void vec2_add_in(Vec2* const to, const Vec2* const what);

/**
 * @brief
 *
 * @param[in/out] from
 * @param[in] what
 *
 * @api
 */
void vec2_sub_in(Vec2* const from, const Vec2* const what);

/**
 * @brief
 *
 * @param[in/out] to
 * @param[in] factor
 *
 * @api
 */
void vec2_scale_in(Vec2* const to, const float factor);

// void vec2_cross_in(Vec2* const to, const Vec2 *const b);

/**
 * @brief
 *
 * @param[in/out] a
 *
 * @api
 */
void vec2_normalize_in(Vec2* const a);
