#include "ivec3.h"
#include <math.h>

void ivec3_init(i32 x, i32 y, i32 z, IVec3* const out) {
    out->x = x;
    out->y = y;
    out->z = z;
}

void ivec3_add(const IVec3* const a, const IVec3* const b, IVec3* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

void ivec3_sub(const IVec3* const a, const IVec3* const b, IVec3* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

void ivec3_scale(const IVec3* const a, const float factor, IVec3* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
    out->z = a->z * factor;
}

void ivec3_cross(const IVec3* const a, const IVec3* const b, IVec3* const out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

float ivec3_dot(const IVec3* const a, const IVec3* const b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float ivec3_length(const IVec3* const a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

void ivec3_normalize(const IVec3* const a, IVec3* const out) {
    ivec3_scale(a, 1.0f / ivec3_length(a), out);
}

float ivec3_distance(const IVec3* const a, const IVec3* const b) {
    IVec3 delta;
    ivec3_sub(a, b, &delta);
    return ivec3_length(&delta);
}


void ivec3_add_in(IVec3* const to, const IVec3* const what) {
    to->x += what->x;
    to->y += what->y;
    to->z += what->z;
}

void ivec3_sub_in(IVec3* const from, const IVec3* const what) {
    from->x -= what->x;
    from->y -= what->y;
    from->z -= what->z;
}

void ivec3_scale_in(IVec3* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
    to->z *= factor;
}

void ivec3_normalize_in(IVec3* const a) {
    float length = ivec3_length(a);
    ivec3_scale_in(a, 1.0f / length);
}


// TODO: Add tests
