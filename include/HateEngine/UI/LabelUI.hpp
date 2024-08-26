//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include "ObjectUI.hpp"

namespace HateEngine {
    class LabelUI : public ObjectUI {
    public:
        // glm::ivec2 size = {100, 100};
        enum TextAlign { Left, Center, Right, Wrap };
        glm::ivec3 color = {0, 0, 0};
        TextAlign text_align = Left;

    public:
        std::string text = "";

        LabelUI(const LabelUI& lbl);
        LabelUI();

        // LabelUI();
    };
} // namespace HateEngine
