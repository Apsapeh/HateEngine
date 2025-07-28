#define HEAPI_LOAD_IMPL
//#define HEAPI_FULL_TRACE
#include <HateEngineRuntimeAPI.h>

#include <stdio.h>
#include <stdlib.h>

void _ready(void) {
    vfs_mount_rfs("/");
    
    printf("Used mem: %zu\n", get_allocated_memory());
    size_t size;
    char* data = vfs_res_read_file("/assets/text.txt", &size);
    
    if (data) {
        printf("Data: %s\n", data);
        tfree(data);
    } else {
        printf("Failed to load data\n");
    }
    printf("Used mem: %zu\n", get_allocated_memory());
    

    printf("Ptr: %p\n", raw_node_new);
    Node* node = node_new("Jopa");
    const char* name = node_get_name(node);

    printf("%s\n", name);

    node_set_name(node, "Node 2");
    name = node_get_name(node);
    printf("%s\n", name);

    Vec3 pos = vec3_new(1, 2, 3);
    vec3_add_in(&pos, &pos);
    printf("%f %f %f\n", pos.x, pos.y, pos.z);
    vec3_normalize_in(&pos);
    printf("%f %f %f\n", pos.x, pos.y, pos.z);

    auto_free((Object*) node);
}

static int count = 0;
void _process(double delta) {
    //printf("Process %d\n", count);
    count++;

    if (count == 20000000) {
        printf("Exit\n");
        exit(0);
    }
}
