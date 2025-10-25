#include "mat4.h"
#include <string.h>

void mat4_init(
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33,
        Mat4* const out
) {
    out->m[0][0] = m00;
    out->m[0][1] = m01;
    out->m[0][2] = m02;
    out->m[0][3] = m03;
    out->m[1][0] = m10;
    out->m[1][1] = m11;
    out->m[1][2] = m12;
    out->m[1][3] = m13;
    out->m[2][0] = m20;
    out->m[2][1] = m21;
    out->m[2][2] = m22;
    out->m[2][3] = m23;
    out->m[3][0] = m30;
    out->m[3][1] = m31;
    out->m[3][2] = m32;
    out->m[3][3] = m33;
}

/*Mat4 mat4_new_zero(void) {
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
}*/

void mat4_from_array(const float* const array, Mat4* const out) {
    memcpy(out, array, sizeof(Mat4));
}

void mat4_clone(const Mat4* const a, Mat4* const out) {
    memcpy(out, a, sizeof(Mat4));
}

void mat4_add(const Mat4* const a, const Mat4* const b, Mat4* const out) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out->m[i][j] = a->m[i][j] + b->m[i][j];
        }
    }
}

void mat4_sub(const Mat4* const a, const Mat4* const b, Mat4* const out) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out->m[i][j] = a->m[i][j] - b->m[i][j];
        }
    }
}

void mat4_mul(const Mat4* const a, const Mat4* const b, Mat4* const out) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            for (int i = 0; i < 4; i++) {
                sum += a->m[row][i] * b->m[i][col];
            }
            out->m[row][col] = sum;
        }
    }
}

// Mat4 mat4_mul_vec

void mat4_scale(const Mat4* const a, const float factor, Mat4* const out) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out->m[i][j] = a->m[i][j] * factor;
        }
    }
}

void mat4_transpose(const Mat4* const a, Mat4* const out) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            out->m[row][col] = a->m[col][row];
        }
    }
}

void mat4_inverse(const Mat4* const a, Mat4* const out) {
    // TOOD: Need a determinant function
    *out = MAT4_ZERO_M;
}


// TODO: Add tests
