#include "uid.h"

UID uid_new(void) {
    static UID uid = 0;
    return uid++;
}
