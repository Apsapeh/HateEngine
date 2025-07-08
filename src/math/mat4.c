#include "mat4.h"
#include <string.h>

Mat4 mat4_new(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
) {
    Mat4 result;
    result.el.m00 = m00;
    result.el.m01 = m01;
    result.el.m02 = m02;
    result.el.m03 = m03;
    result.el.m10 = m10;
    result.el.m11 = m11;
    result.el.m12 = m12;
    result.el.m13 = m13;
    result.el.m20 = m20;
    result.el.m21 = m21;
    result.el.m22 = m22;
    result.el.m23 = m23;
    result.el.m30 = m30;
    result.el.m31 = m31;
    result.el.m32 = m32;
    result.el.m33 = m33;
    return result;
}

Mat4 mat4_new_zero(void) {
    // clang-format off
    float array[16] = {
        0.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 0.0f
    };
    // clang-format on
    return mat4_from_array(array);
}

Mat4 mat4_new_one(void) {
    // clang-format off
    float array[16] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on
    return mat4_from_array(array);
}

Mat4 mat4_from_array(const float* const array) {
    Mat4 result;
    memcpy(&result, array, sizeof(Mat4));
    return result;
}

Mat4 mat4_from_mat4(const Mat4* const a) {
    Mat4 result;
    memcpy(&result, a, sizeof(Mat4));
    return result;
}

Mat4 mat4_add(const Mat4* const a, const Mat4* const b) {
    Mat4 result;
    for (int i = 0; i < 16; i++) {
        result.data[i] = a->data[i] + b->data[i];
    }
    return result;
}

Mat4 mat4_sub(const Mat4* const a, const Mat4* const b) {
    Mat4 result;
    for (int i = 0; i < 16; i++) {
        result.data[i] = a->data[i] - b->data[i];
    }
    return result;
}

Mat4 mat4_mul(const Mat4* const a, const Mat4* const b) {
    Mat4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            for (int i = 0; i < 4; i++) {
                sum += a->data[row * 4 + i] * b->data[i * 4 + col];
            }
            result.data[row * 4 + col] = sum;
        }
    }
    return result;
}

// Mat4 mat4_mul_vec

Mat4 mat4_scale(const Mat4* const a, const float factor) {
    Mat4 result;
    for (int i = 0; i < 16; i++) {
        result.data[i] = a->data[i] * factor;
    }
    return result;
}

Mat4 mat4_transpose(const Mat4* const a) {
    Mat4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.data[row * 4 + col] = a->data[col * 4 + row];
        }
    }
    return result;
}

Mat4 mat4_inverse(const Mat4* const a) {
    // TOOD: Need a determinant function
    return mat4_new_zero();
}


// TODO: Add tests
