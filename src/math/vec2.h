#pragma once

// [[API Generator: forward]]
/**
 * @brief Primitive 2D vector
 *
 */
typedef struct {
    float x;
    float y;
} Vec2;

// [[API Generator]]
Vec2 vec2_new(float x, float y, float z);

// [[API Generator]]
Vec2 vec2_add(const Vec2* const a, const Vec2* const b);

// [[API Generator]]
Vec2 vec2_sub(const Vec2* const a, const Vec2* const b);

// [[API Generator]]
Vec2 vec2_scale(const Vec2* const a, const float factor);

// [[API Generator]]
float vec2_dot(const Vec2* const a, const Vec2* const b);

// [[API Generator]]
float vec2_length(const Vec2* const a);

// [[API Generator]]
Vec2 vec2_normalize(const Vec2* const a);

// [[API Generator]]
float vec2_distance(const Vec2* const a, const Vec2* const b);


// 'in' functions

// [[API Generator]]
/**
 * @brief Add b to a
 *
 * @param a
 * @param b
 */
void vec2_add_in(Vec2* const to, const Vec2* const what);

// [[API Generator]]
void vec2_sub_in(Vec2* const from, const Vec2* const what);

// [[API Generator]]
void vec2_scale_in(Vec2* const to, const float factor);

// void vec2_cross_in(Vec2* const to, const Vec2 *const b);

// [[API Generator]]
void vec2_normalize_in(Vec2* const a);
