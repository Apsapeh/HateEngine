#include "uvec2.h"
#include <math.h>

void uvec2_init(u32 x, u32 y, UVec2* const out) {
    out->x = x;
    out->y = y;
}

void uvec2_add(const UVec2* const a, const UVec2* const b, UVec2* const out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

void uvec2_sub(const UVec2* const a, const UVec2* const b, UVec2* const out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
}

void uvec2_scale(const UVec2* const a, const float factor, UVec2* const out) {
    out->x = a->x * factor;
    out->y = a->y * factor;
}

float uvec2_dot(const UVec2* const a, const UVec2* const b) {
    return a->x * b->x + a->y * b->y;
}

float uvec2_length(const UVec2* const a) {
    return sqrt(a->x * a->x + a->y * a->y);
}

void uvec2_normalize(const UVec2* const a, UVec2* const out) {
    uvec2_scale(a, 1.0f / uvec2_length(a), out);
}

float uvec2_distance(const UVec2* const a, const UVec2* const b) {
    UVec2 delta;
    uvec2_sub(a, b, &delta);
    return uvec2_length(&delta);
}


void uvec2_add_in(UVec2* const to, const UVec2* const what) {
    to->x += what->x;
    to->y += what->y;
}

void uvec2_sub_in(UVec2* const from, const UVec2* const what) {
    from->x -= what->x;
    from->y -= what->y;
}

void uvec2_scale_in(UVec2* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
}

void uvec2_normalize_in(UVec2* const a) {
    float length = uvec2_length(a);
    uvec2_scale_in(a, 1.0f / length);
}


// TODO: Add tests
