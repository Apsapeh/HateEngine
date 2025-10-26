#include "ivec4.h"
#include <math.h>

void ivec4_init(i32 x, i32 y, i32 z, i32 w, IVec4* const out) {
    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;
}

void ivec4_add(const IVec4* const a, const IVec4* const b, IVec4* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
    out->w = a->w + b->w;
}

void ivec4_sub(const IVec4* const a, const IVec4* const b, IVec4* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
    out->w = a->w - b->w;
}

void ivec4_scale(const IVec4* const a, const float factor, IVec4* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
    out->z = a->z * factor;
    out->w = a->w * factor;
}

float ivec4_dot(const IVec4* const a, const IVec4* const b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

float ivec4_length(const IVec4* const a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

void ivec4_normalize(const IVec4* const a, IVec4* const out) {
    ivec4_scale(a, 1.0f / ivec4_length(a), out);
}

float ivec4_distance(const IVec4* const a, const IVec4* const b) {
    IVec4 delta;
    ivec4_sub(a, b, &delta);
    return ivec4_length(&delta);
}


void ivec4_add_in(IVec4* const to, const IVec4* const what) {
    to->x += what->x;
    to->y += what->y;
    to->z += what->z;
    to->w += what->w;
}

void ivec4_sub_in(IVec4* const from, const IVec4* const what) {
    from->x -= what->x;
    from->y -= what->y;
    from->z -= what->z;
    from->w -= what->w;
}

void ivec4_scale_in(IVec4* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
    to->z *= factor;
    to->w *= factor;
}

void ivec4_normalize_in(IVec4* const a) {
    float length = ivec4_length(a);
    ivec4_scale_in(a, 1.0f / length);
}


// TODO: Add tests
