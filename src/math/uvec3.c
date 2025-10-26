#include "uvec3.h"
#include <math.h>

void uvec3_init(u32 x, u32 y, u32 z, UVec3* const out) {
    out->x = x;
    out->y = y;
    out->z = z;
}

void uvec3_add(const UVec3* const a, const UVec3* const b, UVec3* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

void uvec3_sub(const UVec3* const a, const UVec3* const b, UVec3* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

void uvec3_scale(const UVec3* const a, const float factor, UVec3* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
    out->z = a->z * factor;
}

void uvec3_cross(const UVec3* const a, const UVec3* const b, UVec3* const out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

float uvec3_dot(const UVec3* const a, const UVec3* const b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float uvec3_length(const UVec3* const a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

void uvec3_normalize(const UVec3* const a, UVec3* const out) {
    uvec3_scale(a, 1.0f / uvec3_length(a), out);
}

float uvec3_distance(const UVec3* const a, const UVec3* const b) {
    UVec3 delta;
    uvec3_sub(a, b, &delta);
    return uvec3_length(&delta);
}


void uvec3_add_in(UVec3* const to, const UVec3* const what) {
    to->x += what->x;
    to->y += what->y;
    to->z += what->z;
}

void uvec3_sub_in(UVec3* const from, const UVec3* const what) {
    from->x -= what->x;
    from->y -= what->y;
    from->z -= what->z;
}

void uvec3_scale_in(UVec3* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
    to->z *= factor;
}

void uvec3_normalize_in(UVec3* const a) {
    float length = uvec3_length(a);
    uvec3_scale_in(a, 1.0f / length);
}


// TODO: Add tests
