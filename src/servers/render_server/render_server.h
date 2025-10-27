#pragma once

#include <error.h>
#include <types/types.h>
#include "math/ivec2.h"
#include "math/vec4.h"
#include "ex_alloc/chunk_allocator.h"


/*
API ENUM {
        "name": "RenderServerDataOwnMode",
        "type": "char",
        "values": [
                ["Copy", "'\\0'"],
                ["Borrow", "'w'"],
                ["Ptr", "'f'"]
        ]
}
*/

#define RENDER_SERVER_DATA_OWN_MODE_COPY '\0'
#define RENDER_SERVER_DATA_OWN_MODE_BORROW 'w'
#define RENDER_SERVER_DATA_OWN_MODE_PTR 'f'

/**
 * '\0' or 'c' - Data may be copied, may be not, depending on the backend implementation. RenderServer will own this data
 *
 * 'b' - Data will be borrowed. RenderServer will own this data
 *
 * 'p' - Data can be stored as a pointer without copying. Render server is not ownes data
 *
 * @api
 */
typedef u8 RenderServerDataOwnMode;

/**
 * @api
 */
typedef chunk_allocator_ptr RenderServerInstanceCPtr;

/**
 * @api
 */
typedef chunk_allocator_ptr RenderServerMeshCPtr;

/**
 * @api
 */
typedef chunk_allocator_ptr RenderServerMaterialCPtr;

/**
 * @brief 
 * 
 * @api
 */
typedef chunk_allocator_ptr RenderServerTextureCPtr;

/**
 * @api server
 * @api_config {
 *     "fn_prefix": "render_server_",
 *     "init_method": "___hate_engine_runtime_init_render_server"
 * }
 */
typedef struct {
    boolean (*_init)(void);
    boolean (*_quit)(void);
    
    boolean (*frame_begin)(void);
    boolean (*frame_end)(void);
    
    // Environment
    //RID (*environment_create)(void);
    //boolean (*environment_set_ambient_color)(RID rid, Vec4 color);
    //boolean (*environment_destroy)(RID rid);
    
    // Instance
    RenderServerInstanceCPtr (*instance_create)(void);
    boolean (*instance_destroy)(RenderServerInstanceCPtr instance);
    
    // Mesh
    RenderServerMeshCPtr (*mesh_create)(void);
    boolean (*mesh_set_vertices)(RenderServerMeshCPtr ptr, const u8* const data, u64 size, RenderServerDataOwnMode data_own_mode);
    boolean (*mesh_destroy)(RenderServerMeshCPtr ptr);
    
    // Material
    RenderServerMaterialCPtr (*material_create)(void);
    boolean (*material_set_albedo_texture)(RenderServerMaterialCPtr ptr, RenderServerTextureCPtr texture_rid);
    boolean (*material_destroy)(RenderServerMaterialCPtr ptr);
        
    // Texture
    RenderServerTextureCPtr (*texture_create)(void);
    boolean (*texture_set_data)(RenderServerTextureCPtr ptr, const u8* const data, IVec2 dimensions, RenderServerDataOwnMode data_own_mode);
    boolean (*texture_destroy)(RenderServerTextureCPtr ptr);
    
} RenderServerBackend;


// Static global RenderServer
extern RenderServerBackend RenderServer;

/**
 * @brief Initialize the static variables and default backends
 */
void render_server_init(void);

void render_server_exit(void);

/**
 * @brief Register a backend
 * @return "InvalidArgument" if name is NULL or backend is NULL
 * @return "AlreadyExists" if a backend with the same name is already registered
 *
 * @api
 */
boolean render_server_register_backend(const char* name, RenderServerBackend* backend);

/**
 * @brief Load a backend. First you should register them via render_server_register_backend
 * @warning If the backend is already loaded, this function does nothing.
 * @return "InvalidArgument" if name is NULL
 * @return "NotFound" if a backend with the given name is not registered
 * @return "InvalidState" if the backend is already loaded
 *
 * @api
 */
boolean render_server_load_backend(const char* name);

/**
 * @brief If backend was loaded
 */
boolean render_server_is_loaded(void);


/* ====================> RenderServerBackend functions <==================== */

/**
 * @brief Create a new RenderServerBackend instance
 * @return NULL if memory allocation fails
 *
 * @api
 */
RenderServerBackend* render_server_backend_new(void);

/**
 * @brieif Free a RenderServerBackend instance
 * @return "InvalidArgument" if backned is NULL
 *
 * @api
 */
boolean render_server_backend_free(RenderServerBackend* backend);

/**
 * @brief Set a function pointer for a backend
 * @return "InvalidArgument" if name is NULL or func is NULL
 * @return "NotFound" if a function with the given name does not exist in the backend
 *
 * @api
 */
boolean render_server_backend_set_function(
        RenderServerBackend* backend, const char* name, fptr function
);


/**
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 *
 * @api
 */
fptr render_server_backend_get_function(RenderServerBackend* backend, const char* name);
