#pragma once

#include "ObjectUI.hpp"


namespace HateEngine {
    class CheckboxUI : public ObjectUI {
    public:
        // glm::ivec2 size = {100, 100};

        bool is_checked = false;

    public:
        CheckboxUI(const CheckboxUI& checkbox);
        CheckboxUI();

        void set_checked(bool checked);
        bool get_checked();

        // LabelUI();
    };
} // namespace HateEngine
