#include "signal.h"
#include "error.h"
#include "ex_alloc/chunk_allocator.h"
#include "platform/memory.h"
#include "types/types.h"

boolean signal_constructor(Signal* self, const u32 chunk_min_siz, const u8 minimal_chunks_count) {
    return chunk_memory_allocator_constructor(
            &self->allocator, sizeof(SignalCallback), chunk_min_siz, minimal_chunks_count
    );
}

boolean signal_destructor(Signal* self) {
    return chunk_memory_allocator_destructor(&self->allocator);
}

Signal* signal_new_with_params(const u32 chunk_min_size, const u8 minimal_chunks_count) {
    Signal* self = tmalloc(sizeof(Signal));
    ERROR_ALLOC_CHECK(self, { return NULL; });
    boolean s = signal_constructor(self, chunk_min_size, minimal_chunks_count);
    if (s) {
        return self;
    } else {
        tfree(self);
        return NULL;
    }
}

Signal* signal_new(void) {
    return signal_new_with_params(8, 1);
}

boolean signal_free(Signal* self) {
    if (signal_destructor(self)) {
        tfree(self);
        return true;
    } else {
        return false;
    }
}

boolean signal_emit(Signal* self, void* args) {
    ERROR_ARG_CHECK(self, { return false; });

    const usize chunks_count = self->allocator.chunks.size;
    const void** const chunks_data = (const void** const) self->allocator.chunks.data;
    const u8* const bitfield_data = self->allocator.chunks_bitfield.data;
    const u32 chunk_bitfield_size = self->allocator.chunk_bitfield_size;

    for (usize i = 0; i < chunks_count; ++i) {
        const SignalCallback* const chunk = chunks_data[i];
        const u8* const bitfield = bitfield_data + i * chunk_bitfield_size;

        if (!chunk)
            continue;

        for (usize b_i = 0; b_i < chunk_bitfield_size; ++b_i) {
            if (bitfield[b_i] == 0x00)
                continue;
            for (usize bit = 0; bit < 8; ++bit) {
                if (bitfield[b_i] & (1 << bit)) {
                    const SignalCallback* const callback = &chunk[b_i * chunk_bitfield_size + bit];
                    callback->func(args, callback->ctx);
                }
            }
        }
    }

    return true;
}

SignalCallbackHandler signal_connect(Signal* self, SignalCallbackFunc func, void* ctx) {
    ERROR_ARGS_CHECK_2(self, func, { return 0; });

    chunk_allocator_ptr ptr = chunk_memory_allocator_alloc_mem(&self->allocator);
    if (ptr == 0) {
        return 0;
    } else {
        SignalCallback* callback = chunk_memory_allocator_get_real_ptr(&self->allocator, ptr);
        callback->func = func;
        callback->ctx = ctx;
        return ptr;
    }
}

boolean signal_disconnect(Signal* self, SignalCallbackHandler handler) {
    ERROR_ARGS_CHECK_2(self, handler, { return false; });
    return chunk_memory_allocator_free_mem(&self->allocator, handler);
}
