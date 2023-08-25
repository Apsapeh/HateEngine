#include <Old3DEngine/Objects/Physics/PhysicalBody.hpp>
#include <algorithm>

using namespace Old3DEngine;

PhysicalBody::~PhysicalBody() {
    for (ShapeObject &obj : shapes) {
        if (not obj.is_ref)
            delete obj.shape;
    }
    shapes.clear();
}

void PhysicalBody::UpdateBindsPos() {
    for (ControlledObject& obj: binded_objects) {
        obj.obj->setPosition(this->position + glm::vec3(obj.offset.x, obj.offset.y, obj.offset.z));
    }
}

PhysicalBody::BodyType PhysicalBody::getBodyType() {
    return bodyType;
}


UUID_Generator::UUID PhysicalBody::addCollisionShapeClone(CollisionShape shape) {
    UUID_Generator::UUID id = uuidGenerator_shapes.gen();
    CollisionShape *new_shape = new CollisionShape(shape);
    //*new_shape = shape;
    shapes.push_back({new_shape, id, false});
    return id;
}

UUID_Generator::UUID PhysicalBody::addCollisionShapeRef(CollisionShape *shape) {
    UUID_Generator::UUID id = uuidGenerator_shapes.gen();
    shapes.push_back({shape, id, true});
    return id;
}

bool PhysicalBody::delCollisionShape(UUID_Generator::UUID uuid) {
    std::vector<ShapeObject>::iterator iter = std::find_if(
            shapes.begin(), shapes.end(), [&uuid] (ShapeObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != shapes.end()) {
        if (not iter->is_ref)
            delete iter->shape;
        shapes.erase(iter);
        return true;
    }
    return false;
}

UUID_Generator::UUID PhysicalBody::bindObjRef(Object *obj, CoordStruct offset) {
    UUID_Generator::UUID id = uuidGenerator_bindObj.gen();
    binded_objects.push_back({obj, offset, id});
    return id;
}

bool PhysicalBody::unbindObj(UUID_Generator::UUID uuid) {
    std::vector<ControlledObject>::iterator iter = std::find_if(
            binded_objects.begin(), binded_objects.end(), [&uuid] (ControlledObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != binded_objects.end()) {
        binded_objects.erase(iter);
        return true;
    }
    return false;
}

