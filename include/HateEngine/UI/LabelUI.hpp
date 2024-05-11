//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include "ObjectUI.hpp"

namespace HateEngine {
    class LabelUI : public ObjectUI {
    public:
        glm::vec2 size = {100, 100};
        glm::vec4 color = {0, 0, 0, 1};

    public:
        LabelUI();
    };
}