#include <error.h>

// FIXME: It's not thread safety
static Error GLOBAL_ERROR = ERROR_SUCCESS;

void set_error(Error err) {
    GLOBAL_ERROR = err;
}

Error get_error(void) {
    Error tmp = GLOBAL_ERROR;
    GLOBAL_ERROR = ERROR_SUCCESS;
    return tmp;
}
