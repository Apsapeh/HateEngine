#include "vec2.h"
#include <math.h>

Vec2 vec2_new(float x, float y, float z) {
    Vec2 result;
    result.x = x;
    result.y = y;
    return result;
}

Vec2 vec2_add(const Vec2* const a, const Vec2* const b) {
    Vec2 result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    return result;
}

Vec2 vec2_sub(const Vec2* const a, const Vec2* const b) {
    Vec2 result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    return result;
}

Vec2 vec2_scale(const Vec2* const a, const float factor) {
    Vec2 result;
    result.x = a->x * factor;
    result.y = a->y * factor;
    return result;
}

float vec2_dot(const Vec2* const a, const Vec2* const b) {
    return a->x * b->x + a->y * b->y;
}

float vec2_length(const Vec2* const a) {
    return sqrt(a->x * a->x + a->y * a->y);
}

Vec2 vec2_normalize(const Vec2* const a) {
    return vec2_scale(a, 1.0f / vec2_length(a));
}

float vec2_distance(const Vec2* const a, const Vec2* const b) {
    Vec2 delta = vec2_sub(a, b);
    return vec2_length(&delta);
}


void vec2_add_in(Vec2* const to, const Vec2* const what) {
    to->x += what->x;
    to->y += what->y;
}

void vec2_sub_in(Vec2* const from, const Vec2* const what) {
    from->x -= what->x;
    from->y -= what->y;
}

void vec2_scale_in(Vec2* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
}

void vec2_normalize_in(Vec2* const a) {
    float length = vec2_length(a);
    vec2_scale_in(a, 1.0f / length);
}


// TODO: Add tests
