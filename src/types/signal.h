#pragma once

#include <types/types.h>
#include <ex_alloc/chunk_allocator.h>


/**
 * @breif Signal callback template
 *
 * @api
 */
typedef void (*SignalCallbackFunc)(void* args, void* ctx);

typedef struct SignalCallback {
    SignalCallbackFunc func;
    void* ctx;
} SignalCallback;

/**
 * @brief It's just a signal
 *
 * I don't know what I should write here, it's intuitively understandable.
 * You can connect some functions (callbacks) to the signal. Then in other piece of code, you can emit
 * the signal with your args and all callbacks will be executed.
 *
 * @api
 */
typedef struct Signal {
    ChunkMemoryAllocator allocator;
} Signal;

/**
 * @api
 */
typedef chunk_allocator_ptr SignalCallbackHandler;

/**
 * @brief It's like 'signal_new_with_params', but for your allocated Signals. Available only in engine
 *   scope;
 *
 * @error "InvalidArgument"
 * @error "AllocationFailed"
 */
boolean signal_constructor(Signal* self, const u32 chunk_min_siz, const u8 minimal_chunks_count);

/**
 * @brief It's like 'signal_free', but for your allocated Signals. Available only in engine scope.
 *
 * @error "InvalidArgument"
 */
boolean signal_destructor(Signal* self);

/**
 * @brief Create a Signal instance.
 *
 * @error "InvalidArgument"
 * @error "AllocationFailed"
 * @api
 */
Signal* signal_new(void);

/**
 * @brief Create a Signal instance with custom ChunkMemoryAllocator parameters
 *
 * @error "InvalidArgument"
 * @error "AllocationFailed"
 * @api
 */
Signal* signal_new_with_params(const u32 chunk_min_size, const u8 minimal_chunks_count);

/**
 * @brief Free Signal instance
 *
 * @error "InvalidArgument"
 * @api
 */
boolean signal_free(Signal* self);

/**
 * @brief Emit signal. All callbacks will be executed.
 *
 * @error "InvalidArgument"
 * @api
 */
boolean signal_emit(Signal* self, void* args);

/**
 * @breif Connect callback to the signal. It will be executed, when the signal is emitted.
 *
 * @error "InvalidArgument"
 * @error "AllocationFailed"
 * @api
 */
SignalCallbackHandler signal_connect(Signal* self, SignalCallbackFunc func, void* ctx);

/**
 * @brief Disconnect callback from the signal.
 *
 * @error "InvalidArgument"
 * @api
 */
boolean signal_disconnect(Signal* self, SignalCallbackHandler handler);
