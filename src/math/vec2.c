#include "vec2.h"
#include <math.h>

void vec2_init(float x, float y, Vec2* const out) {
    out->x = x;
    out->y = y;
}

void vec2_add(const Vec2* const a, const Vec2* const b, Vec2* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

void vec2_sub(const Vec2* const a, const Vec2* const b, Vec2* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
}

void vec2_scale(const Vec2* const a, const float factor, Vec2* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
}

float vec2_dot(const Vec2* const a, const Vec2* const b) {
    return a->x * b->x + a->y * b->y;
}

float vec2_length(const Vec2* const a) {
    return sqrt(a->x * a->x + a->y * a->y);
}

void vec2_normalize(const Vec2* const a, Vec2* const out) {
    vec2_scale(a, 1.0f / vec2_length(a), out);
}

float vec2_distance(const Vec2* const a, const Vec2* const b) {
    Vec2 delta;
    vec2_sub(a, b, &delta);
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
