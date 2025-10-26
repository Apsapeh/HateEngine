#pragma once

/**
 * @brief Primitive 4D vector
 *
 * @api forward
 */
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec4;

// MACROS API START
#define VEC4_NEW_M(v_x, v_y, v_z, v_w)                                                                  \
    (Vec4) {                                                                                            \
        .x = v_x, .y = v_y, .z = v_z, .w = v_w                                                          \
    }
#define VEC4_NEW_VAL_M(v) VEC4_NEW_M(v, v, v, v)
#define VEC4_ZERO_M VEC4_NEW_VAL_M(0.0f)
#define VEC4_ONE_M VEC4_NEW_VAL_M(1.0f)
#define VEC4_MINUS_ONE_M VEC4_NEW_VAL_M(-1.0f)
// #define VEC4_MAX_M (Vec4){.x=I32_MAX, .y=I32_MAX}
// #define VEC4_MIN_M (Vec4){.x=I32_MIN, .y=I32_MIN}
// MACROS API END

/**
 * @brief
 *
 * @api
 */
void vec4_init(float x, float y, float z, float w, Vec4* const out);

/**
 * @brief
 *
 * @api
 */
void vec4_add(const Vec4* const a, const Vec4* const b, Vec4* const out);

/**
 * @brief
 *
 * @api
 */
void vec4_sub(const Vec4* const a, const Vec4* const b, Vec4* const out);

/**
 * @brief
 *
 * @api
 */
void vec4_scale(const Vec4* const a, const float factor, Vec4* const out);

/**
 * @brief
 *
 * @api
 */
float vec4_dot(const Vec4* const a, const Vec4* const b);

/**
 * @brief
 *
 * @api
 */
float vec4_length(const Vec4* const a);

/**
 * @brief
 *
 * @api
 */
void vec4_normalize(const Vec4* const a, Vec4* const out);

/**
 * @brief
 *
 * @api
 */
float vec4_distance(const Vec4* const a, const Vec4* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
void vec4_add_in(Vec4* const to, const Vec4* const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
void vec4_sub_in(Vec4* const from, const Vec4* const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
void vec4_scale_in(Vec4* const to, const float factor);

// void vec4_cross_in(Vec4* const to, const Vec4 *const b);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
void vec4_normalize_in(Vec4* const a);
