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

/**
 * @brief
 *
 * @api
 */
Vec3 vec3_new(float x, float y, float z);

/**
 * @brief
 *
 * @api
 */
Vec3 vec3_add(const Vec3* const a, const Vec3* const b);

/**
 * @brief
 *
 * @api
 */
Vec3 vec3_sub(const Vec3* const a, const Vec3* const b);

/**
 * @brief
 *
 * @api
 */
Vec3 vec3_scale(const Vec3* const a, const float factor);

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
Vec3 vec3_cross(const Vec3* const a, const Vec3* const b);

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
Vec3 vec3_normalize(const Vec3* const a);

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
