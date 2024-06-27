//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include "../Utilities/UUID.hpp"
#include <glm/glm.hpp>
#include "CoordsUI.hpp"

namespace HateEngine {
    class ObjectUI {
        friend class OpenGL15;
        UUID uuid;

    public:
        enum Type {
            Widget,
            Label
        };
        
    private:
        Type type;

    protected:
        ObjectUI(Type type);
        

    public:
        CoordsUI position = {0, 0};
        CoordsUI size     = {0, 0};
        
        void setScale(float scale);
        void zoom(float delta);

        UUID getUUID() const;
    };
}
