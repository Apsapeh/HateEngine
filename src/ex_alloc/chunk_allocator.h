#pragma once

#include <types/types.h>


/**
 * @brief Virtual memory ptr
 *
 * @api
 */
typedef u32 chunk_allocator_ptr;

/**
 * @brief Chunk memory allocator for elements with fixed type and size
 *
 * Specially optimized allocator for small types, like thousands allocations/freeing of int in each frame
 *
 * @api
 */
typedef struct ChunkMemoryAllocator ChunkMemoryAllocator;

/**
 * @param chunk_min_size Minimal size (in element) of a chunk. It will be aligned to 8. 27 -> 32; 95 -> 96; 256 -> 256
 * 
 * @api
 */
ChunkMemoryAllocator* chunk_memory_allocator_new(u32 element_size, u32 chunk_min_size);

/**
 * @api
 */
void chunk_memory_allocator_free(ChunkMemoryAllocator* this);

/**
 * @return 0 on failure
 * 
 * @api
 */
chunk_allocator_ptr chunk_memory_allocator_alloc_mem(ChunkMemoryAllocator *this);

/**
 * @api
 */
void chunk_memory_allocator_free_mem(ChunkMemoryAllocator* this, chunk_allocator_ptr ptr);

/**
 * @brief Return a real pointer to data
 * @warning You to owned this pointer
 * 
 * @api
 */
void* chunk_memory_allocator_get_real_ptr(ChunkMemoryAllocator* this, chunk_allocator_ptr ptr);

