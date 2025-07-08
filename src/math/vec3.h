#pragma once

// [[API Generator: forward]]
/**
 * @brief Primitive 3D vector
 *
 */
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

// [[API Generator]]
Vec3 vec3_new(float x, float y, float z);

// [[API Generator]]
Vec3 vec3_add(const Vec3* const a, const Vec3* const b);

// [[API Generator]]
Vec3 vec3_sub(const Vec3* const a, const Vec3* const b);

// [[API Generator]]
Vec3 vec3_scale(const Vec3* const a, const float factor);

// [[API Generator]]
float vec3_dot(const Vec3* const a, const Vec3* const b);

// [[API Generator]]
Vec3 vec3_cross(const Vec3* const a, const Vec3* const b);

// [[API Generator]]
float vec3_length(const Vec3* const a);

// [[API Generator]]
Vec3 vec3_normalize(const Vec3* const a);

// [[API Generator]]
float vec3_distance(const Vec3* const a, const Vec3* const b);


// 'in' functions

// [[API Generator]]
/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 */
void vec3_add_in(Vec3* const to, const Vec3* const what);

// [[API Generator]]
void vec3_sub_in(Vec3* const from, const Vec3* const what);

// [[API Generator]]
void vec3_scale_in(Vec3* const to, const float factor);

// void vec3_cross_in(Vec3* const to, const Vec3 *const b);

// [[API Generator]]
void vec3_normalize_in(Vec3* const a);
