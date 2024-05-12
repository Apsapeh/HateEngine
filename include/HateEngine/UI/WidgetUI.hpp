//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include "ObjectUI.hpp"
#include "../Utilities/UUID.hpp"
#include <unordered_map>

namespace HateEngine {
    class WidgetUI : public ObjectUI {
        struct Element {
            ObjectUI* obj;
            bool is_ref;
        };

        std::unordered_map<UUID, Element> elements;

    public:
        glm::vec2 size = {100, 100};
        glm::ivec4 color = {0, 0, 0, 125};
        bool is_movable = true;
        bool is_resizable = true;
        bool is_interactive = true;



    public:
        WidgetUI();

        UUID addObjectRef(ObjectUI* obj);

        bool removeObjectRef(const UUID& uuid);
    };
}