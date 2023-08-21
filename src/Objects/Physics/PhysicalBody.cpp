#include <Old3DEngine/Objects/Physics/PhysicalBody.hpp>

using namespace Old3DEngine;


PhysicalBody::~PhysicalBody() {

}

UUID_Generator::UUID PhysicalBody::addCollisionShapeClone(CollisionShape shape) {
    return UUID_Generator::UUID();
}

UUID_Generator::UUID PhysicalBody::addCollisionShapeRef(CollisionShape *shape) {
    return UUID_Generator::UUID();
}

bool PhysicalBody::delCollisionShape(UUID_Generator::UUID id) {
    return false;
}
