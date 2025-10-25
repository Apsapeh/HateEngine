#pragma once

/**
 * @brief Primitive 3D vector
 *
 * @api forward
 */
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

// MACROS API START
#define VEC3_NEW_M(v_x, v_y, v_z)                                                                       \
    (Vec3) {                                                                                            \
        .x = v_x, .y = v_y, .z = v_z                                                                    \
    }
#define VEC3_ZERO_M VEC3_NEW_M(0.0f, 0.0f, 0.0f)
#define VEC3_ONE_M VEC3_NEW_M(1.0f, 1.0f, 1.0f)
#define VEC3_MINUS_ONE_M VEC3_NEW_M(1.0f, -1.0f, -1.0f)
// #define VEC3_MAX_M (Vec3){.x=I32_MAX, .y=I32_MAX}
// #define VEC3_MIN_M (Vec3){.x=I32_MIN, .y=I32_MIN}
// MACROS API END

/**
 * @brief
 *
 * @api
 */
void vec3_init(float x, float y, float z, Vec3* const out);

/**
 * @brief
 *
 * @api
 */
void vec3_add(const Vec3* const a, const Vec3* const b, Vec3* const out);

/**
 * @brief
 *
 * @api
 */
void vec3_sub(const Vec3* const a, const Vec3* const b, Vec3* const out);

/**
 * @brief
 *
 * @api
 */
void vec3_scale(const Vec3* const a, const float factor, Vec3* const out);

/**
 * @brief
 *
 * @api
 */
void vec3_cross(const Vec3* const a, const Vec3* const b, Vec3* const out);

/**
 * @brief
 *
 * @api
 */
float vec3_dot(const Vec3* const a, const Vec3* const b);

/**
 * @brief
 *
 * @api
 */
float vec3_length(const Vec3* const a);

/**
 * @brief
 *
 * @api
 */
void vec3_normalize(const Vec3* const a, Vec3* const out);

/**
 * @brief
 *
 * @api
 */
float vec3_distance(const Vec3* const a, const Vec3* const b);


// 'in' functions

/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 *
 * @api
 */
void vec3_add_in(Vec3* const to, const Vec3* const what);

/**
 * @brief Subtrtact 'what' from 'from'
 *
 * @param from
 * @param what
 *
 * @api
 */
void vec3_sub_in(Vec3* const from, const Vec3* const what);

/**
 * @brief Scale vector 'to' by 'factor'
 *
 * @param to
 * @param factor
 *
 * @api
 */
void vec3_scale_in(Vec3* const to, const float factor);

// void vec3_cross_in(Vec3* const to, const Vec3 *const b);

/**
 * @brief
 *
 * @param a
 *
 * @api
 */
void vec3_normalize_in(Vec3* const a);
