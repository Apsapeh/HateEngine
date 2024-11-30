//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include <glm/glm.hpp>
#include "../Utilities/UUID.hpp"
#include "CoordsUI.hpp"
#include "../Resources/UIFont.hpp"


namespace HateEngine {
    class ObjectUI {
        friend class OpenGL15;
        UUID uuid;

    public:
        enum Type { Widget, Label, Button, Checkbox, Image };

        std::string text = "";
        glm::ivec4 color = {255, 255, 255, 255};
        glm::ivec4 text_color = {0, 0, 0, 255};

    private:
        Type type;

    protected:
        UIFont* font = nullptr;


        ObjectUI(const ObjectUI& obj);
        ObjectUI(Type type);

    public:
        CoordsUI position = {0, 0};
        CoordsUI size = {0, 0};

        void setScale(float scale);
        void zoom(float delta);

        void setFont(UIFont* font, bool autosize = false);
        void removeFont();
        UIFont* getFont() const;

        void setText(const std::string& text);
        std::string getText() const;

        UUID getUUID() const;
    };
} // namespace HateEngine
