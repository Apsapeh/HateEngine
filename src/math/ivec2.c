#include "ivec2.h"
#include <math.h>

void ivec2_init(i32 x, i32 y, IVec2* const out) {
    out->x = x;
    out->y = y;
}

void ivec2_add(const IVec2* const a, const IVec2* const b, IVec2* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

void ivec2_sub(const IVec2* const a, const IVec2* const b, IVec2* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
}

void ivec2_scale(const IVec2* const a, const float factor, IVec2* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
}

float ivec2_dot(const IVec2* const a, const IVec2* const b) {
    return a->x * b->x + a->y * b->y;
}

float ivec2_length(const IVec2* const a) {
    return sqrt(a->x * a->x + a->y * a->y);
}

void ivec2_normalize(const IVec2* const a, IVec2* const out) {
    ivec2_scale(a, 1.0f / ivec2_length(a), out);
}

float ivec2_distance(const IVec2* const a, const IVec2* const b) {
    IVec2 delta;
    ivec2_sub(a, b, &delta);
    return ivec2_length(&delta);
}


void ivec2_add_in(IVec2* const to, const IVec2* const what) {
    to->x += what->x;
    to->y += what->y;
}

void ivec2_sub_in(IVec2* const from, const IVec2* const what) {
    from->x -= what->x;
    from->y -= what->y;
}

void ivec2_scale_in(IVec2* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
}

void ivec2_normalize_in(IVec2* const a) {
    float length = ivec2_length(a);
    ivec2_scale_in(a, 1.0f / length);
}


// TODO: Add tests
