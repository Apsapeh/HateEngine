#include <Old3DEngine/Objects/Physics/PhysicalBody.hpp>
#include <algorithm>

using namespace Old3DEngine;

PhysicalBody::BodyType PhysicalBody::getBodyType() {
    return bodyType;
}

PhysicalBody::~PhysicalBody() {
    for (ShapeObject &obj : shapes) {
        if (not obj.is_ref)
            delete obj.shape;
    }
    shapes.clear();
}

UUID_Generator::UUID PhysicalBody::addCollisionShapeClone(CollisionShape shape) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    CollisionShape *new_shape = new CollisionShape(shape);
    //*new_shape = shape;
    shapes.push_back({new_shape, id, false});
    return id;
}

UUID_Generator::UUID PhysicalBody::addCollisionShapeRef(CollisionShape *shape) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    shapes.push_back({shape, id, true});
    return id;
}

bool PhysicalBody::delCollisionShape(UUID_Generator::UUID uuid) {
    std::vector<ShapeObject>::iterator iter = std::find_if(
            shapes.begin(), shapes.end(), [&uuid] (ShapeObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != shapes.end() and iter->id == uuid) {
        if (not iter->is_ref)
            delete iter->shape;
        shapes.erase(iter);
        return true;
    }
    return false;
}

