#pragma once

// [[API Generator: forward]]
/**
 * @brief Primitive 4x4 matrix
 * Elements - el.m<row><column>
 * Raw data - data
 */
typedef union {
    struct {
        float m00;
        float m01;
        float m02;
        float m03;
        float m10;
        float m11;
        float m12;
        float m13;
        float m20;
        float m21;
        float m22;
        float m23;
        float m30;
        float m31;
        float m32;
        float m33;
    } el;
    float data[16];
} Mat4;

// [[API Generator]]
Mat4 mat4_new(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);

// [[API Generator]]
Mat4 mat4_new_zero(void);

// [[API Generator]]
Mat4 mat4_new_one(void);

// [[API Generator]]
Mat4 mat4_from_array(const float* const array);

// [[API Generator]]
Mat4 mat4_from_mat4(const Mat4* const a);

// [[API Generator]]
Mat4 mat4_add(const Mat4* const a, const Mat4* const b);

// [[API Generator]]
Mat4 mat4_sub(const Mat4* const a, const Mat4* const b);

// [[API Generator]]
Mat4 mat4_mul(const Mat4* const a, const Mat4* const b);

/// [[API Generator]]
// Mat4 mat4_mul_vec

// [[API Generator]]
Mat4 mat4_scale(const Mat4* const a, const float factor);

// [[API Generator]]
Mat4 mat4_transpose(const Mat4* const a);

// [[API Generator]]
Mat4 mat4_inverse(const Mat4* const a);
