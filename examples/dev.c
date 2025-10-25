#include <math.h>
#define HEAPI_LOAD_IMPL
// #define HEAPI_FULL_TRACE
#include <HateEngineAPI.h>

#include <stdio.h>
#include <stdlib.h>

WindowServerWindow* g_win;
RenderContextSurface* g_surface;

WindowServerWindow* g_win2;
RenderContextSurface* g_surface2;
void _ready(void) {
    vfs_mount_rfs("/");

    string* str_hello = string_from("Hello, ");
    string* str_world = string_from("world!!!!");
    string* str_hello_world;
    string_new(&str_hello_world);

    string_push_back(str_hello_world, str_hello);
    string_push_back(str_hello_world, str_world);

    string_free(str_hello);
    string_free(str_world);

    printf("String hello_world: %s\n", string_cstr(str_hello_world));

    string* str_hello_world_hand = string_from("Hello, world!!!!");

    printf("String equals: %b\n", string_equals(str_hello_world, str_hello_world_hand));

    string_free(str_hello_world);
    string_free(str_hello_world_hand);


    printf("wscw: %p\n", (void*) raw_window_server_create_window);
    printf("wswss: %p\n", (void*) raw_window_server_window_set_size);
    g_win = window_server_create_window("Hello", IVEC2_NEW_M(800, 600), NULL);
    if (!g_win) {
        printf("СМЭРТЬ: %s\n", get_error());
        exit(1);
    }

    g_surface = render_context_create_surface(g_win);
    if (!g_surface) {
        printf("СМЭРТЬ: %s\n", get_error());
        exit(1);
    }

    g_win2 = window_server_create_window("Hello2", IVEC2_NEW_M(800, 600), NULL);
    if (!g_win2) {
        printf("СМЭРТЬ: %s\n", get_error());
        exit(1);
    }

    g_surface2 = render_context_create_surface(g_win2);
    if (!g_surface2) {
        printf("СМЭРТЬ: %s\n", get_error());
        exit(1);
    }
    // window_server_destroy_window(win);*/

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

    printf("Ptr: %p\n", (void*) raw_node_new);
    Node* node = node_new("Jopa");
    const char* name = node_get_name(node);

    printf("%s\n", name);

    node_set_name(node, "Node 2");
    name = node_get_name(node);
    printf("%s\n", name);

    Vec3 pos = VEC3_NEW_M(1, 2, 3);
    vec3_add_in(&pos, &pos);
    printf("%f %f %f\n", pos.x, pos.y, pos.z);
    vec3_normalize_in(&pos);
    printf("%f %f %f\n", pos.x, pos.y, pos.z);


    auto_free((Object*) node);
    vfs_unmount_rfs();
}

static int g_count = 0;
double g_time = 0;
void _process(double delta) {
    // printf("Process %d\n", count);
    g_time += delta;
    g_count++;

    // if (count % 1000 == 0) {
    window_server_window_set_size(g_win, IVEC2_NEW_M(800 + 100 * sin(g_time), 600));
    window_server_window_set_size(g_win2, IVEC2_NEW_M(800, 600 + 75 * cos(g_time)));
    // x}

    render_context_surface_present(g_surface);
    render_context_surface_present(g_surface2);

    return;

    if (g_count == 20000000) {
        printf("Exit\n");
        exit(0);
    }
}
