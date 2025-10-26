#include "chunk_allocator.h"
#include <types/vector.h>
#include <platform/memory.h>
#include <log.h>
#include <error.h>

#define CHUNK_COUNT_DEFAULT 4


// clang-format off
vector_template_def_static(chunkUsageBitfieldPlain, u8)
vector_template_impl_static(chunkUsageBitfieldPlain, u8)
struct DummyClangFormat; // This code dirty hack for normal formatting. It's does nothing for program
// clang-format on

/*
 * Chunk stores as SOA.
 * vec_ptr chunks is an array of allocated chunks.
 * chunk_usage_bitfield is an array of bitfields. Each chunk contains u8[chunk_size / 8]
 */

struct ChunkMemoryAllocator {
    vec_ptr chunks;
    vec_chunkUsageBitfieldPlain chunks_bitfield_usage;
    u32 chunk_bitfield_size; // Size of bitfield in bytes
    u32 chunk_element_count; // Size of chunk in element
    u32 element_size; // Size of each element
    u32 freed_chunks; // Number of freed chunks
};


/**
 * @param chunk_min_size Minimal size (in element) of a chunk.
 * It will be aligned to 8. 27 -> 32; 95 -> 96; 256 -> 256
 *
 * @api
 */
ChunkMemoryAllocator* chunk_memory_allocator_new(u32 element_size, u32 chunk_min_size) {
    if (element_size == 0 || chunk_min_size == 0) {
        LOG_ERROR_OR_DEBUG_FATAL("chung_memory_allocator_new: 'elemnt_size' or 'chunk_min_size' is 0");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    ChunkMemoryAllocator* allocator = tmalloc(sizeof(ChunkMemoryAllocator));
    ERROR_ALLOC_CHECK(allocator, { return NULL; });

    allocator->freed_chunks = 0;
    allocator->element_size = element_size;
    allocator->chunk_bitfield_size = chunk_min_size >> 3;
    if (chunk_min_size % 8 != 0)
        allocator->chunk_bitfield_size++;
    allocator->chunk_element_count = allocator->chunk_bitfield_size << 3;

    // Init chunk array
    allocator->chunks = vec_ptr_init();
    vec_ptr_reserve(&allocator->chunks, CHUNK_COUNT_DEFAULT);

    for (usize i = 0; i < CHUNK_COUNT_DEFAULT; i++) {
        void* chunk = tmalloc(allocator->chunk_element_count * allocator->element_size);
        ERROR_ALLOC_CHECK(chunk, {
            for (usize j = 0; j < i; j++) {
                tfree(allocator->chunks.data[j]);
            }
            vec_ptr_free(&allocator->chunks);
            tfree(allocator);
            return NULL;
        });
        vec_ptr_push_back(&allocator->chunks, chunk);
    }

    // Init bitfield array
    allocator->chunks_bitfield_usage = vec_chunkUsageBitfieldPlain_init();
    // const usize chunk_bitfield_size = allocator->chunk_bitfield_size >> 3;
    vec_chunkUsageBitfieldPlain_reserve(
            &allocator->chunks_bitfield_usage, CHUNK_COUNT_DEFAULT * allocator->chunk_bitfield_size
    );

    for (usize i = 0; i < CHUNK_COUNT_DEFAULT * allocator->chunk_bitfield_size; i++) {
        vec_chunkUsageBitfieldPlain_push_back(&allocator->chunks_bitfield_usage, 0);
    }

    return allocator;
}

/**
 * @api
 */
void chunk_memory_allocator_free(ChunkMemoryAllocator* this) {
    for (usize i = 0; i < this->chunks.size; i++) {
        tfree(this->chunks.data[i]);
    }
    vec_ptr_free(&this->chunks);
    vec_chunkUsageBitfieldPlain_free(&this->chunks_bitfield_usage);
    tfree(this);
}

/**
 * @api
 */
chunk_allocator_ptr chunk_memory_allocator_alloc_mem(ChunkMemoryAllocator* this) {
    ERROR_ARG_CHECK(this, return 0;);
    // Find a free chunk
    for (usize c_i = 0; c_i < this->chunks.size; c_i++) {
        u8* bitfield = &this->chunks_bitfield_usage.data[c_i * this->chunk_bitfield_size];
        for (usize b_i = 0; b_i < this->chunk_bitfield_size; b_i++) {
            if (bitfield[b_i] != 0xFF) {
#if (defined(__GNUC__) || defined(__clang__))
                usize free_bit = __builtin_ctz(~bitfield[b_i]);
                bitfield[b_i] |= 1 << free_bit;
                return c_i * this->chunk_element_count + b_i * 8 + free_bit + 1;
#else
                usize free_bit = 0;
                while (bitfield[b_i] & (1 << free_bit)) {
                    free_bit++;
                }
                bitfield[b_i] |= 1 << free_bit;
                return c_i * this->chunk_element_count + b_i * 8 + free_bit + 1;
#endif
            }
        }
    }

    // No free chunk found, allocate a new one
    void* chunk = tmalloc(this->chunk_element_count * this->element_size);
    ERROR_ALLOC_CHECK(chunk, { return 0; });
    if (!vec_ptr_push_back(&this->chunks, chunk)) {
        tfree(chunk);
        LOG_ERROR("Chunk vector add failed");
        set_error(ERROR_ALLOCATION_FAILED);
        return 0;
    }

    if (this->freed_chunks > 0) {
        this->freed_chunks--;
    }

    for (usize i = 0; i < this->chunk_bitfield_size; i++) {
        if (!vec_chunkUsageBitfieldPlain_push_back(&this->chunks_bitfield_usage, 0)) {
            tfree(chunk);
            vec_ptr_pop_back(&this->chunks);
            vec_chunkUsageBitfieldPlain_erase_range(
                    &this->chunks_bitfield_usage, this->chunks_bitfield_usage.size - i, i
            );
            LOG_ERROR("Chunk vector bitfield add failed");
            set_error(ERROR_ALLOCATION_FAILED);
            return 0;
        }
    }

    this->chunks_bitfield_usage.data[(this->chunks.size - 1) * this->chunk_bitfield_size] = 1;
    return (this->chunks.size - 1) * this->chunk_element_count + 1;
}

/**
 * @api
 */
void chunk_memory_allocator_free_mem(ChunkMemoryAllocator* this, chunk_allocator_ptr ptr) {
    ERROR_ARGS_CHECK_2(this, ptr, );

    --ptr;
    if (ptr >= this->chunk_element_count * this->chunks.size) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid pointer");
        set_error(ERROR_INVALID_ARGUMENT);
        return;
    }

    const usize chunk_index = ptr / this->chunk_element_count;
    const usize element_in_chunk = ptr % this->chunk_element_count;
    const usize bitfield_index = chunk_index * this->chunk_bitfield_size;
    const usize byte_index = element_in_chunk / 8;
    const usize bit_index = element_in_chunk % 8;
    const u8 bit_mask = 1 << bit_index;

    this->chunks_bitfield_usage.data[bitfield_index + byte_index] &= ~bit_mask;

    for (usize c_i = this->chunks.size; c_i-- > CHUNK_COUNT_DEFAULT;) {
        boolean is_empty = true;
        for (usize i = 0; i < this->chunk_bitfield_size; ++i) {
            if (this->chunks_bitfield_usage.data[c_i * this->chunk_bitfield_size + i] != 0) {
                is_empty = false;
                break;
            }
        }

        if (!is_empty)
            break;

        tfree(this->chunks.data[c_i]);
        vec_ptr_pop_back(&this->chunks);
        vec_chunkUsageBitfieldPlain_erase_range(
                &this->chunks_bitfield_usage, c_i * this->chunk_bitfield_size, this->chunk_bitfield_size
        );
        ++this->freed_chunks;
    }

    if (this->freed_chunks > this->chunks.size) {
        vec_ptr_shrink_to_fit(&this->chunks);
        vec_chunkUsageBitfieldPlain_shrink_to_fit(&this->chunks_bitfield_usage);
        this->freed_chunks = 0;
    }
}

/**
 * @brief Return a real pointer to data
 * @warning You to owned this pointer
 *
 * @api
 */
void* chunk_memory_allocator_get_real_ptr(ChunkMemoryAllocator* this, chunk_allocator_ptr ptr) {
    ERROR_ARGS_CHECK_2(this, ptr, { return NULL; });

    --ptr;

    if (ptr >= this->chunk_element_count * this->chunks.size) {
        LOG_ERROR_OR_DEBUG_FATAL("Invalid pointer");
        set_error(ERROR_INVALID_ARGUMENT);
        return NULL;
    }

    const usize chunk_index = ptr / this->chunk_element_count;
    const usize element_in_chunk = ptr % this->chunk_element_count;
    const usize bitfield_index = chunk_index * this->chunk_bitfield_size;
    const usize byte_index = element_in_chunk / 8;
    const usize bit_index = element_in_chunk % 8;
    const u8 bit_mask = 1 << bit_index;

    if (this->chunks_bitfield_usage.data[bitfield_index + byte_index] & bit_mask) {
        return (u8*) this->chunks.data[chunk_index] + element_in_chunk * this->element_size;
    }
    return NULL;
}
