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

// MACROS API START
#define VEC2_NEW_M(v_x, v_y)                                                                            \
    (Vec2) {                                                                                            \
        .x = v_x, .y = v_y                                                                              \
    }
#define VEC2_NEW_VAL_M(v) VEC2_NEW_M(v, v)
#define VEC2_ZERO_M VEC2_NEW_VAL_M(0.0f)
#define VEC2_ONE_M VEC2_NEW_VAL_M(1.0f)
#define VEC2_MINUS_ONE_M VEC2_NEW_VAL_M(-1.0f)
// #define VEC2_MAX_M (Vec2){.x=I32_MAX, .y=I32_MAX}
// #define VEC2_MIN_M (Vec2){.x=I32_MIN, .y=I32_MIN}

// MACROS API END

/**
 * @api
 */
void vec2_init(const float x, const float y, Vec2* const out);

/**
 * @api
 */
void vec2_add(const Vec2* const a, const Vec2* const b, Vec2* const out);

/**
 * @api
 */
void vec2_sub(const Vec2* const a, const Vec2* const b, Vec2* const out);

/**
 * @api
 */
void vec2_scale(const Vec2* const a, const float factor, Vec2* const out);

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
void vec2_normalize(const Vec2* const a, Vec2* const out);

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
