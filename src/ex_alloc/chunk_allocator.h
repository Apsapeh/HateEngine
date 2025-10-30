#pragma once

#include <types/types.h>
#include <types/vector.h>


/**
 * @brief Virtual memory ptr
 *
 * @warning 0 is not valid value
 *
 * @api
 */
typedef u32 chunk_allocator_ptr;

/*
 * Chunk stores as SOA.
 * vec_ptr chunks is an array of allocated chunks.
 * chunk_usage_bitfield is an array of bitfields. Each chunk contains u8[chunk_size / 8]
 */


/**
 * @brief Chunk memory allocator for elements with fixed type and size
 *
 * Specially optimized allocator for small types, like thousands allocations/freeing of int in each frame
 *
 * @api
 */
typedef struct ChunkMemoryAllocator {
    vec_ptr chunks;
    vec_u8 chunks_bitfield;
    u32 chunk_bitfield_size; // Size of bitfield in bytes
    u32 chunk_element_count; // Size of chunk in element
    u32 element_size; // Size of each element
    u32 freed_chunks; // Number of freed chunks
    u8 minimal_chunks_count;
} ChunkMemoryAllocator;

/**
 *
 */
boolean chunk_memory_allocator_constructor(
        ChunkMemoryAllocator* self, const u32 element_size, const u32 chunk_min_size,
        const u8 minimal_chunks_count
);

boolean chunk_memory_allocator_destructor(ChunkMemoryAllocator* self);

/**
 * @param chunk_min_size Minimal size (in element) of a chunk. It will be aligned to 8. 27 -> 32;
 * 95 -> 96; 256 -> 256
 *
 * @param minimal_chunks_count Count of chunks will never be lower than that value
 *
 * @return Pointer to allocated memory on succes, 0 on failure
 *
 * @error "InvalidArgument"
 * @error "AllocationFailed"
 * @api
 */
ChunkMemoryAllocator* chunk_memory_allocator_new(
        const u32 element_size, const u32 chunk_min_size, const u8 minimal_chunks_count
);

/**
 *
 * @error "InvalidArgument"
 * @api
 */
boolean chunk_memory_allocator_free(ChunkMemoryAllocator* this);

/**
 * @return 0 on failure
 *
 *
 * @error "InvalidArgument"
 * @error "AllocationFailed" Real memory can't be allocated
 * @api
 */
chunk_allocator_ptr chunk_memory_allocator_alloc_mem(ChunkMemoryAllocator* this);

/**
 * @error "InvalidArgument"
 * @api
 */
boolean chunk_memory_allocator_free_mem(ChunkMemoryAllocator* this, chunk_allocator_ptr ptr);

/**
 * @brief Return a real pointer to data
 * @warning You to owned this pointer
 *
 * @error "InvalidArgument"
 * @api
 */
void* chunk_memory_allocator_get_real_ptr(ChunkMemoryAllocator* this, chunk_allocator_ptr ptr);
