#pragma once

#include <types/types.h>

/**
 * @brief Opaque mutex handle
 * @api
 */
typedef struct mutex_handle* mutex_handle;

/**
 * @brief Create a new mutex
 * @return Mutex handle or NULL on failure
 * @warning Must be freed with mutex_free()
 * @api
 */
mutex_handle mutex_new(void);

/**
 * @brief Create a new mutex which allows same thread to lock multiple times
 * @return Mutex handle or NULL on failure
 * @warning Must be freed with mutex_free()
 * @api
 */
mutex_handle mutex_new_recursive(void);

/**
 * @brief Free a mutex
 * @param handle Valid mutex handle
 * @warning Behavior undefined if mutex is locked
 * @api
 */
void mutex_free(mutex_handle handle);

/**
 * @brief Lock a mutex (blocking)
 * @param handle Valid mutex handle
 * @warning Undefined behavior if handle is NULL
 * @api
 */
void mutex_lock(mutex_handle handle);

/**
 * @brief Try to lock a mutex without blocking
 * @param handle Valid mutex handle
 * @return true if locked successfully, false if already locked
 * @api
 */
boolean mutex_try_lock(mutex_handle handle);

/**
 * @brief Try to lock with timeout
 * @param handle Valid mutex handle
 * @param timeout_ms Timeout in milliseconds (0 = try_lock, UINT32_MAX = infinite)
 * @return true if locked, false on timeout
 * @api
 */
// boolean mutex_lock_timeout(mutex_handle handle, u32 timeout_ms);

/**
 * @brief Unlock a mutex
 * @param handle Valid mutex handle
 * @warning Must be called by the thread that locked it
 * @api
 */
void mutex_unlock(mutex_handle handle);
