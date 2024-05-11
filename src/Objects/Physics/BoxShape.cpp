//
// Created by Тимофей Кириченко on 30.04.2024.
//

#include <HateEngine/Objects/Physics/BoxShape.hpp>

using namespace HateEngine;

BoxShape::BoxShape(glm::vec3 extents, glm::vec3 pos, glm::vec3 rot) : CollisionShape(Box, pos, rot) {
    changeSize(extents.x, extents.y, extents.z);
}

void BoxShape::changeSize(float width, float height, float length) {
    this->reactRightSize = {length/2, height/2, width/2};

    if (reactShape != nullptr) {
        ((reactphysics3d::BoxShape*)reactShape)->setHalfExtents(
            {reactRightSize.x, reactRightSize.y, reactRightSize.z}
        );
    }
}
