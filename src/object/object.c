#include "object.h"
#include "types/uid.h"

void object_init(Object* this, enum ObjectType type) {
    this->type = type;
    this->uid = uid_new();
}

void object_free(Object* object) {
}
