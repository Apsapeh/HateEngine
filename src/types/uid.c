#include "uid.h"
#include "platform/mutex.h"

static UID g_uid = 0;
static mutex_handle g_mutex;


void uid_init(void) {
    g_mutex = mutex_new();
}

void uid_exit(void) {
    mutex_free(g_mutex);
}

UID uid_new(void) {
    mutex_lock(g_mutex);
    UID uid = g_uid++;
    mutex_unlock(g_mutex);
    return uid;
}
