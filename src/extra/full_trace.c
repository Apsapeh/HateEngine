
#include <log.h>
#include "full_trace.h"

inline bool full_trace_vfs_mount_res(const char *___file___, int ___line___, const char* path, const char* mount_point) {
    he_update_full_trace_info("vfs_mount_res", ___file___, ___line___);
    bool result = vfs_mount_res(path, mount_point);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_unmount_res(const char *___file___, int ___line___, const char* path, const char* mount_point) {
    he_update_full_trace_info("vfs_unmount_res", ___file___, ___line___);
    bool result = vfs_unmount_res(path, mount_point);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_mount_rfs(const char *___file___, int ___line___, const char* mount_point) {
    he_update_full_trace_info("vfs_mount_rfs", ___file___, ___line___);
    bool result = vfs_mount_rfs(mount_point);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_mount_rfs_whitelist(const char *___file___, int ___line___, const char** whitelist, size_t count, const char* mount_point) {
    he_update_full_trace_info("vfs_mount_rfs_whitelist", ___file___, ___line___);
    bool result = vfs_mount_rfs_whitelist(whitelist, count, mount_point);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_unmount_rfs(const char *___file___, int ___line___) {
    he_update_full_trace_info("vfs_unmount_rfs", ___file___, ___line___);
    bool result = vfs_unmount_rfs();
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_res_path_exists(const char *___file___, int ___line___, const char* path) {
    he_update_full_trace_info("vfs_res_path_exists", ___file___, ___line___);
    bool result = vfs_res_path_exists(path);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_usr_path_exists(const char *___file___, int ___line___, const char* path, bool prefer_res) {
    he_update_full_trace_info("vfs_usr_path_exists", ___file___, ___line___);
    bool result = vfs_usr_path_exists(path, prefer_res);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void* full_trace_vfs_res_read_file(const char *___file___, int ___line___, const char* path, size_t* size) {
    he_update_full_trace_info("vfs_res_read_file", ___file___, ___line___);
    void* result = vfs_res_read_file(path, size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline FileStream* full_trace_vfs_res_stream_open(const char *___file___, int ___line___, const char* path) {
    he_update_full_trace_info("vfs_res_stream_open", ___file___, ___line___);
    FileStream* result = vfs_res_stream_open(path);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void* full_trace_vfs_usr_read_file(const char *___file___, int ___line___, const char* path, size_t* size, bool prefer_res) {
    he_update_full_trace_info("vfs_usr_read_file", ___file___, ___line___);
    void* result = vfs_usr_read_file(path, size, prefer_res);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_usr_write_file(const char *___file___, int ___line___, const char* path, const void* data, size_t size) {
    he_update_full_trace_info("vfs_usr_write_file", ___file___, ___line___);
    bool result = vfs_usr_write_file(path, data, size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline FileStream* full_trace_vfs_usr_stream_open(const char *___file___, int ___line___, const char* path, bool prefer_res) {
    he_update_full_trace_info("vfs_usr_stream_open", ___file___, ___line___);
    FileStream* result = vfs_usr_stream_open(path, prefer_res);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline size_t full_trace_vfs_stream_size(const char *___file___, int ___line___, FileStream* stream) {
    he_update_full_trace_info("vfs_stream_size", ___file___, ___line___);
    size_t result = vfs_stream_size(stream);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline size_t full_trace_vfs_stream_read_n(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size) {
    he_update_full_trace_info("vfs_stream_read_n", ___file___, ___line___);
    size_t result = vfs_stream_read_n(stream, buffer, size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void* full_trace_vfs_stream_read_all(const char *___file___, int ___line___, FileStream* stream, size_t* size) {
    he_update_full_trace_info("vfs_stream_read_all", ___file___, ___line___);
    void* result = vfs_stream_read_all(stream, size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline size_t full_trace_vfs_stream_write(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size) {
    he_update_full_trace_info("vfs_stream_write", ___file___, ___line___);
    size_t result = vfs_stream_write(stream, buffer, size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_stream_seek(const char *___file___, int ___line___, FileStream* stream, enum FSSeekFrom whence, size_t offset) {
    he_update_full_trace_info("vfs_stream_seek", ___file___, ___line___);
    bool result = vfs_stream_seek(stream, whence, offset);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline size_t full_trace_vfs_stream_tell(const char *___file___, int ___line___, FileStream* stream, bool* success) {
    he_update_full_trace_info("vfs_stream_tell", ___file___, ___line___);
    size_t result = vfs_stream_tell(stream, success);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_vfs_stream_flush(const char *___file___, int ___line___, FileStream* stream) {
    he_update_full_trace_info("vfs_stream_flush", ___file___, ___line___);
    bool result = vfs_stream_flush(stream);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vfs_stream_close(const char *___file___, int ___line___, FileStream* stream) {
    he_update_full_trace_info("vfs_stream_close", ___file___, ___line___);
    vfs_stream_close(stream);
    he_update_full_trace_info("", "", -1);
}

inline UID full_trace_uid_new(const char *___file___, int ___line___) {
    he_update_full_trace_info("uid_new", ___file___, ___line___);
    UID result = uid_new();
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void* full_trace_tmalloc(const char *___file___, int ___line___, size_t size) {
    he_update_full_trace_info("tmalloc", ___file___, ___line___);
    void* result = tmalloc(size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void* full_trace_trealloc(const char *___file___, int ___line___, void* ptr, size_t size) {
    he_update_full_trace_info("trealloc", ___file___, ___line___);
    void* result = trealloc(ptr, size);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_tfree(const char *___file___, int ___line___, void* ptr) {
    he_update_full_trace_info("tfree", ___file___, ___line___);
    tfree(ptr);
    he_update_full_trace_info("", "", -1);
}

inline size_t full_trace_get_allocated_memory(const char *___file___, int ___line___) {
    he_update_full_trace_info("get_allocated_memory", ___file___, ___line___);
    size_t result = get_allocated_memory();
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_new(const char *___file___, int ___line___, 
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
) {
    he_update_full_trace_info("mat4_new", ___file___, ___line___);
    Mat4 result = mat4_new(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_new_zero(const char *___file___, int ___line___) {
    he_update_full_trace_info("mat4_new_zero", ___file___, ___line___);
    Mat4 result = mat4_new_zero();
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_new_one(const char *___file___, int ___line___) {
    he_update_full_trace_info("mat4_new_one", ___file___, ___line___);
    Mat4 result = mat4_new_one();
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_from_array(const char *___file___, int ___line___, const float* const array) {
    he_update_full_trace_info("mat4_from_array", ___file___, ___line___);
    Mat4 result = mat4_from_array(array);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_from_mat4(const char *___file___, int ___line___, const Mat4* const a) {
    he_update_full_trace_info("mat4_from_mat4", ___file___, ___line___);
    Mat4 result = mat4_from_mat4(a);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_add(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b) {
    he_update_full_trace_info("mat4_add", ___file___, ___line___);
    Mat4 result = mat4_add(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_sub(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b) {
    he_update_full_trace_info("mat4_sub", ___file___, ___line___);
    Mat4 result = mat4_sub(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_mul(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b) {
    he_update_full_trace_info("mat4_mul", ___file___, ___line___);
    Mat4 result = mat4_mul(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_scale(const char *___file___, int ___line___, const Mat4* const a, const float factor) {
    he_update_full_trace_info("mat4_scale", ___file___, ___line___);
    Mat4 result = mat4_scale(a, factor);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_transpose(const char *___file___, int ___line___, const Mat4* const a) {
    he_update_full_trace_info("mat4_transpose", ___file___, ___line___);
    Mat4 result = mat4_transpose(a);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Mat4 full_trace_mat4_inverse(const char *___file___, int ___line___, const Mat4* const a) {
    he_update_full_trace_info("mat4_inverse", ___file___, ___line___);
    Mat4 result = mat4_inverse(a);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_new(const char *___file___, int ___line___, float x, float y, float z) {
    he_update_full_trace_info("vec3_new", ___file___, ___line___);
    Vec3 result = vec3_new(x, y, z);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_add(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b) {
    he_update_full_trace_info("vec3_add", ___file___, ___line___);
    Vec3 result = vec3_add(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_sub(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b) {
    he_update_full_trace_info("vec3_sub", ___file___, ___line___);
    Vec3 result = vec3_sub(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_scale(const char *___file___, int ___line___, const Vec3* const a, const float factor) {
    he_update_full_trace_info("vec3_scale", ___file___, ___line___);
    Vec3 result = vec3_scale(a, factor);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec3_dot(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b) {
    he_update_full_trace_info("vec3_dot", ___file___, ___line___);
    float result = vec3_dot(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_cross(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b) {
    he_update_full_trace_info("vec3_cross", ___file___, ___line___);
    Vec3 result = vec3_cross(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec3_length(const char *___file___, int ___line___, const Vec3* const a) {
    he_update_full_trace_info("vec3_length", ___file___, ___line___);
    float result = vec3_length(a);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Vec3 full_trace_vec3_normalize(const char *___file___, int ___line___, const Vec3* const a) {
    he_update_full_trace_info("vec3_normalize", ___file___, ___line___);
    Vec3 result = vec3_normalize(a);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline float full_trace_vec3_distance(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b) {
    he_update_full_trace_info("vec3_distance", ___file___, ___line___);
    float result = vec3_distance(a, b);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_vec3_add_in(const char *___file___, int ___line___, Vec3* const to, const Vec3* const what) {
    he_update_full_trace_info("vec3_add_in", ___file___, ___line___);
    vec3_add_in(to, what);
    he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_sub_in(const char *___file___, int ___line___, Vec3* const from, const Vec3* const what) {
    he_update_full_trace_info("vec3_sub_in", ___file___, ___line___);
    vec3_sub_in(from, what);
    he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_scale_in(const char *___file___, int ___line___, Vec3* const to, const float factor) {
    he_update_full_trace_info("vec3_scale_in", ___file___, ___line___);
    vec3_scale_in(to, factor);
    he_update_full_trace_info("", "", -1);
}

inline void full_trace_vec3_normalize_in(const char *___file___, int ___line___, Vec3* const a) {
    he_update_full_trace_info("vec3_normalize_in", ___file___, ___line___);
    vec3_normalize_in(a);
    he_update_full_trace_info("", "", -1);
}

inline void full_trace_auto_free(const char *___file___, int ___line___, Object* object) {
    he_update_full_trace_info("auto_free", ___file___, ___line___);
    auto_free(object);
    he_update_full_trace_info("", "", -1);
}

inline Node* full_trace_node_new(const char *___file___, int ___line___, const char* name) {
    he_update_full_trace_info("node_new", ___file___, ___line___);
    Node* result = node_new(name);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Node* full_trace_from_node(const char *___file___, int ___line___, Node* node) {
    he_update_full_trace_info("from_node", ___file___, ___line___);
    Node* result = from_node(node);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_node_set_name(const char *___file___, int ___line___, Node* node, const char* name) {
    he_update_full_trace_info("node_set_name", ___file___, ___line___);
    node_set_name(node, name);
    he_update_full_trace_info("", "", -1);
}

inline const char* full_trace_node_get_name(const char *___file___, int ___line___, Node* node) {
    he_update_full_trace_info("node_get_name", ___file___, ___line___);
    const char* result = node_get_name(node);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline void full_trace_node_add_child(const char *___file___, int ___line___, Node* node, Node* child) {
    he_update_full_trace_info("node_add_child", ___file___, ___line___);
    node_add_child(node, child);
    he_update_full_trace_info("", "", -1);
}

inline bool full_trace_node_remove_child(const char *___file___, int ___line___, Node* node, Node* child) {
    he_update_full_trace_info("node_remove_child", ___file___, ___line___);
    bool result = node_remove_child(node, child);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_node_remove_child_by_name(const char *___file___, int ___line___, Node* node, const char* name) {
    he_update_full_trace_info("node_remove_child_by_name", ___file___, ___line___);
    bool result = node_remove_child_by_name(node, name);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_node_remove_child_by_uid(const char *___file___, int ___line___, Node* node, UID uid) {
    he_update_full_trace_info("node_remove_child_by_uid", ___file___, ___line___);
    bool result = node_remove_child_by_uid(node, uid);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline bool full_trace_node_remove_all_children(const char *___file___, int ___line___, Node* node) {
    he_update_full_trace_info("node_remove_all_children", ___file___, ___line___);
    bool result = node_remove_all_children(node);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Node* full_trace_node_get_child_by_name(const char *___file___, int ___line___, Node* node, const char* name) {
    he_update_full_trace_info("node_get_child_by_name", ___file___, ___line___);
    Node* result = node_get_child_by_name(node, name);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Node* full_trace_node_get_child_by_uid(const char *___file___, int ___line___, Node* node, UID uid) {
    he_update_full_trace_info("node_get_child_by_uid", ___file___, ___line___);
    Node* result = node_get_child_by_uid(node, uid);
    he_update_full_trace_info("", "", -1);
    return result;
}

inline Node3D* full_trace_node3d_new(const char *___file___, int ___line___) {
    he_update_full_trace_info("node3d_new", ___file___, ___line___);
    Node3D* result = node3d_new();
    he_update_full_trace_info("", "", -1);
    return result;
}


