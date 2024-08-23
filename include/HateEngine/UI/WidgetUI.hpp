//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include <unordered_map>
#include "../Utilities/UUID.hpp"
#include "ButtonUI.hpp"
#include "CheckboxUI.hpp"
#include "LabelUI.hpp"
#include "ObjectUI.hpp"

namespace HateEngine {
    class WidgetUI : public ObjectUI {
        friend class OpenGL15;
        struct Element {
            ObjectUI* obj;
            bool is_ref;
        };

        std::unordered_map<UUID, Element> elements;

    public:
        // glm::vec2 size = {100, 100};
        glm::ivec4 color = {0, 0, 0, 125};
        std::string title = "Widget";
        bool has_border = false;
        bool is_movable = false;
        bool is_scalable = false;
        bool is_closable = false;
        bool is_minimizable = false;
        bool has_scrollbar = false;
        bool has_title = false;
        bool is_scroll_autohide = false;
        bool has_background = true;
        bool is_scalable_left = false;
        bool has_input = true;

        bool is_interactive = true;

    public:
        WidgetUI();

        UUID addObjectRef(ObjectUI* obj);
        UUID addObjectClone(LabelUI& lbl);
        UUID addObjectClone(ButtonUI& obj);
        UUID addObjectClone(CheckboxUI& ckbox);

        bool removeObjectRef(const UUID& uuid);
    };
} // namespace HateEngine
