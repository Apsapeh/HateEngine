#include <error.h>
#include <log.h>
#include <platform/thread_local_storage.h>

// FIXME: It's not thread safety
// static Error GLOBAL_ERROR = ERROR_SUCCESS;

// clang-format off
thread_local_storage_impl(Error, g_errorRegister)
struct DummyClangFormat; // This code dirty hack for normal formatting. It's does nothing for program
// clang-format on

boolean __error_check_ptr_arg(void* ptr, c_str name) {
    if (ptr) {
        return false;
    } else {
        LOG_ERROR("Invalid argument (is NULL): ", name);
        set_error(ERROR_INVALID_ARGUMENT);
        return true;
    }
}

void set_error(Error err) {
    g_errorRegister_set_value(err);
}

Error get_error(void) {
    g_errorRegister_try_init(ERROR_SUCCESS);

    Error tmp = g_errorRegister_get_value();
    g_errorRegister_set_value(ERROR_SUCCESS);
    return tmp;
}
