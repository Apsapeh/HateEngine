#pragma once

#include "../HateEngine.hpp"
#include "ObjectUI.hpp"

namespace HateEngine {
    class CheckboxUI : public ObjectUI {
    public:
        enum TextAlign { Left, Center, Right };

        // glm::ivec2 size = {100, 100};
        void (*on_click)(HateEngine::Engine* engine) = nullptr;
        glm::ivec3 color = {0, 0, 0};
        TextAlign text_align = Left;
        bool is_checked = false;

    public:
        std::string text = "";

        CheckboxUI();
        
        void set_checked(bool checked);
        bool get_checked() const;

        // LabelUI();
    };
} // namespace HateEngine
