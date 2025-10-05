#pragma once

/**
 * @brief Primitive 4x4 matrix
 *
 * Raw data - m
 *
 * @api forward
 */
typedef struct {
    float m[4][4];
} Mat4;

/**
 * @brief
 *
 * @param m00
 * @param m01
 * @param m02
 * @param m03
 * @param m10
 * @param m11
 * @param m12
 * @param m13
 * @param m20
 * @param m21
 * @param m22
 * @param m23
 * @param m30
 * @param m31
 * @param m32
 * @param m33
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_new(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);

/**
 * @brief
 *
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_new_zero(void);

/**
 * @brief
 *
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_new_one(void);

/**
 * @brief
 *
 * @param array
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_from_array(const float* const array);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_from_mat4(const Mat4* const a);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_add(const Mat4* const a, const Mat4* const b);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_sub(const Mat4* const a, const Mat4* const b);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_mul(const Mat4* const a, const Mat4* const b);

/**
 * @brief
 *
 * @param a
 * @param factor
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_scale(const Mat4* const a, const float factor);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_transpose(const Mat4* const a);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
Mat4 mat4_inverse(const Mat4* const a);
