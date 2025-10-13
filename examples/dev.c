#include <math.h>
#define HEAPI_LOAD_IMPL
#define HEAPI_FULL_TRACE
#include <HateEngineAPI.h>

#include <stdio.h>
#include <stdlib.h>

WindowServerWindow* win;
RenderContextSurface* surface;

WindowServerWindow* win2;
RenderContextSurface* surface2;
void _ready(void) {
    vfs_mount_rfs("/");
    
    printf("wscw: %p\n", raw_window_server_create_window);
    printf("wswss: %p\n", raw_window_server_window_set_size);
    if (window_server_create_window("Hello", 800, 600, NULL, &win)) {
        printf("СМЭРТЬ\n");
        exit(1);
    }
    
    if (render_context_create_surface(win, &surface)) {
        printf("СМЭРТЬ\n");
        exit(1);
    }
    
    if (window_server_create_window("Hello 2", 800, 600, NULL, &win2)) {
        printf("СМЭРТЬ\n");
        exit(1);
    }
    
    if (render_context_create_surface(win2, &surface2)) {
        printf("СМЭРТЬ\n");
        exit(1);
    }
    //window_server_destroy_window(win);*/

    printf("Used mem: %zu\n", get_allocated_memory());
    u64 size;
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
    vfs_unmount_rfs();
}
                 
static int count = 0;
double _time = 0;
void _process(double delta) {
    //printf("Process %d\n", count);
    _time += delta;
    count++;
    
    //if (count % 1000 == 0) {
    window_server_window_set_size(win, 800 + 100 * sin(_time), 600);
    window_server_window_set_size(win2, 800, 600 + 75 * cos(_time));
        //x}
    
    render_context_surface_present(surface);
    render_context_surface_present(surface2);
    
    return;

    if (count == 20000000) {
        printf("Exit\n");
        exit(0);
    }
}
