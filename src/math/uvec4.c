#include "uvec4.h"
#include <math.h>

void uvec4_init(u32 x, u32 y, u32 z, u32 w, UVec4* const out) {
    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;
}

void uvec4_add(const UVec4* const a, const UVec4* const b, UVec4* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
    out->w = a->w + b->w;
}

void uvec4_sub(const UVec4* const a, const UVec4* const b, UVec4* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
    out->w = a->w - b->w;
}

void uvec4_scale(const UVec4* const a, const float factor, UVec4* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
    out->z = a->z * factor;
    out->w = a->w * factor;
}

float uvec4_dot(const UVec4* const a, const UVec4* const b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

float uvec4_length(const UVec4* const a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

void uvec4_normalize(const UVec4* const a, UVec4* const out) {
    uvec4_scale(a, 1.0f / uvec4_length(a), out);
}

float uvec4_distance(const UVec4* const a, const UVec4* const b) {
    UVec4 delta;
    uvec4_sub(a, b, &delta);
    return uvec4_length(&delta);
}


void uvec4_add_in(UVec4* const to, const UVec4* const what) {
    to->x += what->x;
    to->y += what->y;
    to->z += what->z;
    to->w += what->w;
}

void uvec4_sub_in(UVec4* const from, const UVec4* const what) {
    from->x -= what->x;
    from->y -= what->y;
    from->z -= what->z;
    from->w -= what->w;
}

void uvec4_scale_in(UVec4* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
    to->z *= factor;
    to->w *= factor;
}

void uvec4_normalize_in(UVec4* const a) {
    float length = uvec4_length(a);
    uvec4_scale_in(a, 1.0f / length);
}


// TODO: Add tests
