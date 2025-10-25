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

// MACROS API START
// clang-format off
#define MAT4_NEW_M(                                                                                     \
        v_m00, v_m01, v_m02, v_m03, v_m10, v_m11, v_m12, v_m13, v_m20, v_m21, v_m22, v_m23, v_m30,      \
        v_m31, v_m32, v_m33                                                                             \
)                                                                                                       \
    (Mat4) {                                                                                            \
        .m = {                                                                                          \
            v_m00,                                                                                      \
            v_m01,                                                                                      \
            v_m02,                                                                                      \
            v_m03,                                                                                      \
            v_m10,                                                                                      \
            v_m11,                                                                                      \
            v_m12,                                                                                      \
            v_m13,                                                                                      \
            v_m20,                                                                                      \
            v_m21,                                                                                      \
            v_m22,                                                                                      \
            v_m23,                                                                                      \
            v_m30,                                                                                      \
            v_m31,                                                                                      \
            v_m32,                                                                                      \
            v_m33                                                                                       \
        }                                                                                               \
    }

#define MAT4_ZERO_M                                                                                     \
    MAT4_NEW_M(                                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 0.0f                                                                          \
    )
    
#define MAT4_ONE_M                                                                                      \
    MAT4_NEW_M(                                                                                         \
        1.0f, 0.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 1.0f, 0.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 1.0f, 0.0f,                                                                         \
        0.0f, 0.0f, 0.0f, 1.0f                                                                          \
    )
// clang-format on

// MACROS API END

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
 * @param [out] Result Mat4
 *
 * @api
 */
void mat4_init(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33,
        Mat4* const out
);

/**
 * @brief
 *
 * @param array
 * @return Mat4
 *
 * @api
 */
void mat4_from_array(const float* const array, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
void mat4_clone(const Mat4* const a, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
void mat4_add(const Mat4* const a, const Mat4* const b, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
void mat4_sub(const Mat4* const a, const Mat4* const b, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return Mat4
 *
 * @api
 */
void mat4_mul(const Mat4* const a, const Mat4* const b, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @param factor
 * @return Mat4
 *
 * @api
 */
void mat4_scale(const Mat4* const a, const float factor, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
void mat4_transpose(const Mat4* const a, Mat4* const out);

/**
 * @brief
 *
 * @param a
 * @return Mat4
 *
 * @api
 */
void mat4_inverse(const Mat4* const a, Mat4* const out);
