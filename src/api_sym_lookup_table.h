
#include "platform/memory.h"
#include "platform/fs/fs.h"
#include "object/object.h"
#include "object/node/node.h"
#include "object/node/node3d/node3d.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "types/uid.h"
#include "vfs/vfs.h"


typedef struct {
    const char* name;
    void* ptr;
} APIFunctionLookupTable;

APIFunctionLookupTable api_function_lookup_table[] = {
        {"tmalloc", (void*) tmalloc},
        {"trealloc", (void*) trealloc},
        {"tfree", (void*) tfree},
        {"get_allocated_memory", (void*) get_allocated_memory},
        {"auto_free", (void*) auto_free},
        {"node_new", (void*) node_new},
        {"from_node", (void*) from_node},
        {"node_set_name", (void*) node_set_name},
        {"node_get_name", (void*) node_get_name},
        {"node_add_child", (void*) node_add_child},
        {"node_remove_child", (void*) node_remove_child},
        {"node_remove_child_by_name", (void*) node_remove_child_by_name},
        {"node_remove_child_by_uid", (void*) node_remove_child_by_uid},
        {"node_remove_all_children", (void*) node_remove_all_children},
        {"node_get_child_by_name", (void*) node_get_child_by_name},
        {"node_get_child_by_uid", (void*) node_get_child_by_uid},
        {"node3d_new", (void*) node3d_new},
        {"mat4_new", (void*) mat4_new},
        {"mat4_new_zero", (void*) mat4_new_zero},
        {"mat4_new_one", (void*) mat4_new_one},
        {"mat4_from_array", (void*) mat4_from_array},
        {"mat4_from_mat4", (void*) mat4_from_mat4},
        {"mat4_add", (void*) mat4_add},
        {"mat4_sub", (void*) mat4_sub},
        {"mat4_mul", (void*) mat4_mul},
        {"mat4_scale", (void*) mat4_scale},
        {"mat4_transpose", (void*) mat4_transpose},
        {"mat4_inverse", (void*) mat4_inverse},
        {"vec3_new", (void*) vec3_new},
        {"vec3_add", (void*) vec3_add},
        {"vec3_sub", (void*) vec3_sub},
        {"vec3_scale", (void*) vec3_scale},
        {"vec3_dot", (void*) vec3_dot},
        {"vec3_cross", (void*) vec3_cross},
        {"vec3_length", (void*) vec3_length},
        {"vec3_normalize", (void*) vec3_normalize},
        {"vec3_distance", (void*) vec3_distance},
        {"vec3_add_in", (void*) vec3_add_in},
        {"vec3_sub_in", (void*) vec3_sub_in},
        {"vec3_scale_in", (void*) vec3_scale_in},
        {"vec3_normalize_in", (void*) vec3_normalize_in},
        {"uid_new", (void*) uid_new},
        {"vfs_mount_res", (void*) vfs_mount_res},
        {"vfs_unmount_res", (void*) vfs_unmount_res},
        {"vfs_mount_rfs", (void*) vfs_mount_rfs},
        {"vfs_mount_rfs_whitelist", (void*) vfs_mount_rfs_whitelist},
        {"vfs_unmount_rfs", (void*) vfs_unmount_rfs},
        {"vfs_res_path_exists", (void*) vfs_res_path_exists},
        {"vfs_usr_path_exists", (void*) vfs_usr_path_exists},
        {"vfs_res_read_file", (void*) vfs_res_read_file},
        {"vfs_res_stream_open", (void*) vfs_res_stream_open},
        {"vfs_usr_read_file", (void*) vfs_usr_read_file},
        {"vfs_usr_write_file", (void*) vfs_usr_write_file},
        {"vfs_usr_stream_open", (void*) vfs_usr_stream_open},
        {"vfs_stream_size", (void*) vfs_stream_size},
        {"vfs_stream_read_n", (void*) vfs_stream_read_n},
        {"vfs_stream_read_all", (void*) vfs_stream_read_all},
        {"vfs_stream_write", (void*) vfs_stream_write},
        {"vfs_stream_seek", (void*) vfs_stream_seek},
        {"vfs_stream_tell", (void*) vfs_stream_tell},
        {"vfs_stream_flush", (void*) vfs_stream_flush},
        {"vfs_stream_close", (void*) vfs_stream_close},

};
