#include "vec4.h"
#include <math.h>

void vec4_init(float x, float y, float z, float w, Vec4* const out) {
    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;
}

void vec4_add(const Vec4* const a, const Vec4* const b, Vec4* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
    out->w = a->w + b->w;
}

void vec4_sub(const Vec4* const a, const Vec4* const b, Vec4* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
    out->w = a->w - b->w;
}

void vec4_scale(const Vec4* const a, const float factor, Vec4* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
    out->z = a->z * factor;
    out->w = a->w * factor;
}

float vec4_dot(const Vec4* const a, const Vec4* const b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

float vec4_length(const Vec4* const a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

void vec4_normalize(const Vec4* const a, Vec4* const out) {
    vec4_scale(a, 1.0f / vec4_length(a), out);
}

float vec4_distance(const Vec4* const a, const Vec4* const b) {
    Vec4 delta;
    vec4_sub(a, b, &delta);
    return vec4_length(&delta);
}


void vec4_add_in(Vec4* const to, const Vec4* const what) {
    to->x += what->x;
    to->y += what->y;
    to->z += what->z;
    to->w += what->w;
}

void vec4_sub_in(Vec4* const from, const Vec4* const what) {
    from->x -= what->x;
    from->y -= what->y;
    from->z -= what->z;
    from->w -= what->w;
}

void vec4_scale_in(Vec4* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
    to->z *= factor;
    to->w *= factor;
}

void vec4_normalize_in(Vec4* const a) {
    float length = vec4_length(a);
    vec4_scale_in(a, 1.0f / length);
}


// TODO: Add tests
