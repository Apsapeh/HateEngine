#include <HateEngine/Objects/Physics/CollisionShape.hpp>

using namespace HateEngine;

CollisionShape::CollisionShape(CollisionShape::ShapeEnum shape, float param1, float param2, float param3) {
    this->shapeType = shape;
    this->param1 = param1;
    this->param2 = param2;
    this->param3 = param3;
}

CollisionShape::CollisionShape(float radius) {
    this->shapeType = Sphere;
    this->param1 = radius;
}

CollisionShape::CollisionShape(float radius, float height) {
    this->shapeType = Capsule;
    this->param1 = radius;
    this->param2 = height;
}

CollisionShape::CollisionShape(float width, float height, float length) {
    this->shapeType = Box;
    this->param1 = width;
    this->param2 = height;
    this->param3 = length;
}


void CollisionShape::setCollisionLayers(std::vector<uint8_t>& layers, std::vector<bool> state) {
    for (uint8_t i : layers) {
        if (i < 15)
            collisionLayers[i] = state[i];
    }
}

void CollisionShape::setCollisionMasks(std::vector<uint8_t>& masks, std::vector<bool> state) {
    for (uint8_t i : masks) {
        if (i < 15)
            collisionLayers[i] = state[i];
    }
}