#include "vec3.h"
#include <math.h>

Vec3 vec3_new(float x, float y, float z) {
    Vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

Vec3 vec3_add(const Vec3* const a, const Vec3* const b) {
    Vec3 result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

Vec3 vec3_sub(const Vec3* const a, const Vec3* const b) {
    Vec3 result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    result.z = a->z - b->z;
    return result;
}

Vec3 vec3_scale(const Vec3* const a, const float factor) {
    Vec3 result;
    result.x = a->x * factor;
    result.y = a->y * factor;
    result.z = a->z * factor;
    return result;
}

float vec3_dot(const Vec3* const a, const Vec3* const b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vec3 vec3_cross(const Vec3* const a, const Vec3* const b) {
    Vec3 result;
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    return result;
}

float vec3_length(const Vec3* const a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

Vec3 vec3_normalize(const Vec3* const a) {
    return vec3_scale(a, 1.0f / vec3_length(a));
}

float vec3_distance(const Vec3* const a, const Vec3* const b) {
    Vec3 delta = vec3_sub(a, b);
    return vec3_length(&delta);
}


void vec3_add_in(Vec3* const to, const Vec3* const what) {
    to->x += what->x;
    to->y += what->y;
    to->z += what->z;
}

void vec3_sub_in(Vec3* const from, const Vec3* const what) {
    from->x -= what->x;
    from->y -= what->y;
    from->z -= what->z;
}

void vec3_scale_in(Vec3* const to, const float factor) {
    to->x *= factor;
    to->y *= factor;
    to->z *= factor;
}

void vec3_normalize_in(Vec3* const a) {
    float length = vec3_length(a);
    vec3_scale_in(a, 1.0f / length);
}


// TODO: Add tests
