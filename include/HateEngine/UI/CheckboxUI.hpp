#pragma once

#include "ObjectUI.hpp"

namespace HateEngine {
    class CheckboxUI : public ObjectUI {
    public:
        enum TextAlign { Left, Center, Right };

        // glm::ivec2 size = {100, 100};
        glm::ivec3 color = {0, 0, 0};
        TextAlign text_align = Left;
        bool is_checked = false;

    public:
        std::string text = "";

        CheckboxUI(const CheckboxUI &checkbox );
        CheckboxUI();
        
        void set_checked(bool checked);
        bool get_checked();

        // LabelUI();
    };
} // namespace HateEngine
