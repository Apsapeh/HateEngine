
#pragma once
#include "platform/memory.h"
#include "platform/fs/fs.h"
#include "object/object.h"
#include "object/node/node.h"
#include "object/node/node3d/node3d.h"
#include "object/node/window/window.h"
#include "object/node/canvas_item/canvas_item.h"
#include "object/node/canvas_item/control/control.h"
#include "object/node/canvas_item/control/viewport/viewport.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "types/uid.h"
#include "vfs/vfs.h"
#include "servers/window_server/window_server.h"
#include "servers/surface_bridge/surface_bridge.h"


void* full_trace_tmalloc(const char *___file___, int ___line___, size_t size);
void* full_trace_trealloc(const char *___file___, int ___line___, void* ptr, size_t size);
void full_trace_tfree(const char *___file___, int ___line___, void* ptr);
size_t full_trace_get_allocated_memory(const char *___file___, int ___line___);
void full_trace_auto_free(const char *___file___, int ___line___, Object* object);
Node* full_trace_node_new(const char *___file___, int ___line___, const char* name);
Node* full_trace_from_node(const char *___file___, int ___line___, Node* node);
void full_trace_node_set_name(const char *___file___, int ___line___, Node* node, const char* name);
const char* full_trace_node_get_name(const char *___file___, int ___line___, Node* node);
void full_trace_node_add_child(const char *___file___, int ___line___, Node* node, Node* child);
bool full_trace_node_remove_child(const char *___file___, int ___line___, Node* node, Node* child);
bool full_trace_node_remove_child_by_name(const char *___file___, int ___line___, Node* node, const char* name);
bool full_trace_node_remove_child_by_uid(const char *___file___, int ___line___, Node* node, UID uid);
bool full_trace_node_remove_all_children(const char *___file___, int ___line___, Node* node);
Node* full_trace_node_get_child_by_name(const char *___file___, int ___line___, Node* node, const char* name);
Node* full_trace_node_get_child_by_uid(const char *___file___, int ___line___, Node* node, UID uid);
Node3D* full_trace_node3d_new(const char *___file___, int ___line___, const char* name);
Window* full_trace_window_new(const char *___file___, int ___line___, const char* name, const char* title, int w, int h);
void full_trace_window_set_title(const char *___file___, int ___line___, Window* this, const char* title);
const char* full_trace_window_get_title(const char *___file___, int ___line___, const Window* this);
CanvasItem* full_trace_canvas_item_new(const char *___file___, int ___line___, const char* name);
Control* full_trace_control_new(const char *___file___, int ___line___, const char* name);
Viewport* full_trace_viewport_new(const char *___file___, int ___line___, const char* name);
Mat4 full_trace_mat4_new(const char *___file___, int ___line___, 
        float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33
);
Mat4 full_trace_mat4_new_zero(const char *___file___, int ___line___);
Mat4 full_trace_mat4_new_one(const char *___file___, int ___line___);
Mat4 full_trace_mat4_from_array(const char *___file___, int ___line___, const float* const array);
Mat4 full_trace_mat4_from_mat4(const char *___file___, int ___line___, const Mat4* const a);
Mat4 full_trace_mat4_add(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);
Mat4 full_trace_mat4_sub(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);
Mat4 full_trace_mat4_mul(const char *___file___, int ___line___, const Mat4* const a, const Mat4* const b);
Mat4 full_trace_mat4_scale(const char *___file___, int ___line___, const Mat4* const a, const float factor);
Mat4 full_trace_mat4_transpose(const char *___file___, int ___line___, const Mat4* const a);
Mat4 full_trace_mat4_inverse(const char *___file___, int ___line___, const Mat4* const a);
Vec3 full_trace_vec3_new(const char *___file___, int ___line___, float x, float y, float z);
Vec3 full_trace_vec3_add(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
Vec3 full_trace_vec3_sub(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
Vec3 full_trace_vec3_scale(const char *___file___, int ___line___, const Vec3* const a, const float factor);
float full_trace_vec3_dot(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
Vec3 full_trace_vec3_cross(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
float full_trace_vec3_length(const char *___file___, int ___line___, const Vec3* const a);
Vec3 full_trace_vec3_normalize(const char *___file___, int ___line___, const Vec3* const a);
float full_trace_vec3_distance(const char *___file___, int ___line___, const Vec3* const a, const Vec3* const b);
void full_trace_vec3_add_in(const char *___file___, int ___line___, Vec3* const to, const Vec3* const what);
void full_trace_vec3_sub_in(const char *___file___, int ___line___, Vec3* const from, const Vec3* const what);
void full_trace_vec3_scale_in(const char *___file___, int ___line___, Vec3* const to, const float factor);
void full_trace_vec3_normalize_in(const char *___file___, int ___line___, Vec3* const a);
Vec2 full_trace_vec2_new(const char *___file___, int ___line___, float x, float y, float z);
Vec2 full_trace_vec2_add(const char *___file___, int ___line___, const Vec2* const a, const Vec2* const b);
Vec2 full_trace_vec2_sub(const char *___file___, int ___line___, const Vec2* const a, const Vec2* const b);
Vec2 full_trace_vec2_scale(const char *___file___, int ___line___, const Vec2* const a, const float factor);
float full_trace_vec2_dot(const char *___file___, int ___line___, const Vec2* const a, const Vec2* const b);
float full_trace_vec2_length(const char *___file___, int ___line___, const Vec2* const a);
Vec2 full_trace_vec2_normalize(const char *___file___, int ___line___, const Vec2* const a);
float full_trace_vec2_distance(const char *___file___, int ___line___, const Vec2* const a, const Vec2* const b);
void full_trace_vec2_add_in(const char *___file___, int ___line___, Vec2* const to, const Vec2* const what);
void full_trace_vec2_sub_in(const char *___file___, int ___line___, Vec2* const from, const Vec2* const what);
void full_trace_vec2_scale_in(const char *___file___, int ___line___, Vec2* const to, const float factor);
void full_trace_vec2_normalize_in(const char *___file___, int ___line___, Vec2* const a);
UID full_trace_uid_new(const char *___file___, int ___line___);
bool full_trace_vfs_mount_res(const char *___file___, int ___line___, const char* path, const char* mount_point);
bool full_trace_vfs_unmount_res(const char *___file___, int ___line___, const char* path, const char* mount_point);
bool full_trace_vfs_mount_rfs(const char *___file___, int ___line___, const char* mount_point);
bool full_trace_vfs_mount_rfs_whitelist(const char *___file___, int ___line___, const char** whitelist, size_t count, const char* mount_point);
bool full_trace_vfs_unmount_rfs(const char *___file___, int ___line___);
bool full_trace_vfs_res_path_exists(const char *___file___, int ___line___, const char* path);
bool full_trace_vfs_usr_path_exists(const char *___file___, int ___line___, const char* path, bool prefer_res);
void* full_trace_vfs_res_read_file(const char *___file___, int ___line___, const char* path, size_t* size);
FileStream* full_trace_vfs_res_stream_open(const char *___file___, int ___line___, const char* path);
void* full_trace_vfs_usr_read_file(const char *___file___, int ___line___, const char* path, size_t* size, bool prefer_res);
bool full_trace_vfs_usr_write_file(const char *___file___, int ___line___, const char* path, const void* data, size_t size);
FileStream* full_trace_vfs_usr_stream_open(const char *___file___, int ___line___, const char* path, bool prefer_res);
size_t full_trace_vfs_stream_size(const char *___file___, int ___line___, FileStream* stream);
size_t full_trace_vfs_stream_read_n(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size);
void* full_trace_vfs_stream_read_all(const char *___file___, int ___line___, FileStream* stream, size_t* size);
size_t full_trace_vfs_stream_write(const char *___file___, int ___line___, FileStream* stream, void* buffer, size_t size);
bool full_trace_vfs_stream_seek(const char *___file___, int ___line___, FileStream* stream, enum FSSeekFrom whence, size_t offset);
size_t full_trace_vfs_stream_tell(const char *___file___, int ___line___, FileStream* stream, bool* success);
bool full_trace_vfs_stream_flush(const char *___file___, int ___line___, FileStream* stream);
void full_trace_vfs_stream_close(const char *___file___, int ___line___, FileStream* stream);
Error full_trace_window_server_register_backend(const char *___file___, int ___line___, const char* name, WindowServerBackend* backend);
Error full_trace_window_server_load_backend(const char *___file___, int ___line___, const char* name);
WindowServerBackend* full_trace_window_server_backend_new(const char *___file___, int ___line___);
Error full_trace_window_server_backend_set_function(const char *___file___, int ___line___, 
        WindowServerBackend* backend, const char* name, void (*function)(void)
);
Error full_trace_window_server_register_backend(const char *___file___, int ___line___, const char* name, WindowServerBackend* backend);
Error full_trace_window_server_load_backend(const char *___file___, int ___line___, const char* name);
SurfaceBridgeBackend* full_trace_surface_bridge_backend_new(const char *___file___, int ___line___);
Error full_trace_surface_bridge_backend_set_function(const char *___file___, int ___line___, 
        SurfaceBridgeBackend* backend, const char* name, void (*function)(void)
);

