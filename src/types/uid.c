#include "uid.h"

UID uid_new(void) {
    // FIXME: Add mutex
    static UID uid = 0;
    return uid++;
}
