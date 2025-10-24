#include <error.h>
#include <platform/thread_local_storage.h>

// FIXME: It's not thread safety
// static Error GLOBAL_ERROR = ERROR_SUCCESS;

// clang-format off
thread_local_storage_impl(Error, g_errorRegister)
struct DummyClangFormat; // This code dirty hack for normal formatting. It's does nothing for program
// clang-format on

void set_error(Error err) {
    g_errorRegister_set_value(err);
}

Error get_error(void) {
    g_errorRegister_try_init(ERROR_SUCCESS);

    Error tmp = g_errorRegister_get_value();
    g_errorRegister_set_value(ERROR_SUCCESS);
    return tmp;
}
