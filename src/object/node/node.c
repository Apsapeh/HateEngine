#include "node.h"
#include <stddef.h>
#include <string.h>
#include "object/object.h"
#include "platform/memory.h"

void node_init(Node* this, enum ObjectType object_type, const char* name) {
    object_init(&this->object, object_type);
    this->children = vec_ptr_init();
    this->name = NULL;
    node_set_name(this, name);
}

void node_free(Node* this) {
    object_free(&this->object);
    vec_ptr_free(&this->children);
    tfree(this->name);
}


Node* node_new(const char* name) {
    Node* node = tmalloc(sizeof(Node));
    node_init(node, ObjectTypeNode, name);
    return node;
}

Node* from_node(Node* node) {
    Node* new_node = node_new(node->name);
    return new_node;
}

void node_set_name(Node* this, const char* name) {
    if (this->name != NULL) {
        tfree(this->name);
    }

    size_t len = strlen(name) + 1;
    this->name = tmalloc(len);
    memcpy(this->name, name, len);
}

const char* node_get_name(Node* node) {
    return node->name;
}

void node_add_child(Node* node, Node* child) {
    vec_ptr_push_back(&node->children, child);
}

bool node_remove_child(Node* node, Node* child) {
    // vec_ptr_remove(&node->children, child);
    const size_t size = node->children.size;
    Node** ptr = (Node**) node->children.data;
    for (size_t i = 0; i < size; i++) {
        if (ptr[i] == child) {
            auto_free((Object*) ptr[i]);
            vec_ptr_erase(&node->children, i);
            return true;
        }
    }
    return false;
}

bool node_remove_child_by_name(Node* node, const char* name) {
    const size_t size = node->children.size;
    Node** ptr = (Node**) node->children.data;
    for (size_t i = 0; i < size; i++) {
        if (strcmp(ptr[i]->name, name) == 0) {
            auto_free((Object*) ptr[i]);
            vec_ptr_erase(&node->children, i);
            return true;
        }
    }
    return false;
}

bool node_remove_child_by_uid(Node* node, UID uid) {
    const size_t size = node->children.size;
    Object** ptr = (Object**) node->children.data;
    for (size_t i = 0; i < size; i++) {
        if (ptr[i]->uid == uid) {
            auto_free(ptr[i]);
            vec_ptr_erase(&node->children, i);
            return true;
        }
    }
    return false;
}

bool node_remove_all_children(Node* node) {
    const size_t size = node->children.size;
    Object** ptr = (Object**) node->children.data;
    for (size_t i = 0; i < size; i++) {
        auto_free(ptr[i]);
    }
    vec_ptr_clear(&node->children);
    return false;
}

Node* node_get_child_by_name(Node* node, const char* name) {
    const size_t size = node->children.size;
    Node** ptr = (Node**) node->children.data;
    for (size_t i = 0; i < size; i++) {
        if (strcmp(ptr[i]->name, name) == 0) {
            return ptr[i];
        }
    }
    return NULL;
}

Node* node_get_child_by_uid(Node* node, UID uid) {
    const size_t size = node->children.size;
    Object** ptr = (Object**) node->children.data;
    for (size_t i = 0; i < size; i++) {
        if (ptr[i]->uid == uid) {
            return (Node*) ptr[i];
        }
    }
    return NULL;
}
