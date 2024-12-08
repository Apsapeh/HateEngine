//
// Created by Тимофей Кириченко on 30.04.2024.
//

#pragma once

#include "CollisionShape.hpp"

namespace HateEngine {

    class BoxShape : public CollisionShape {
    public:
        //                      legth/2, width/2, height/2
        glm::vec3 reactRightSize = {.5f, .5f, .5f};

    public:
        BoxShape(
                glm::vec3 extents = {1.0f, 1.0f, 1.0f}, glm::vec3 pos = {0, 0, 0},
                glm::vec3 rot = {0, 0, 0}
        );

        void changeSize(float width, float height, float length);
    };

} // namespace HateEngine
