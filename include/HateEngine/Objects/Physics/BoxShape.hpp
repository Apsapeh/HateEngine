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
    BoxShape(float width, float height, float length);

    void changeSize(float width, float height, float length);
};

} // HateEngine

