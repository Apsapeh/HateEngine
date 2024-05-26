//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include "../Utilities/UUID.hpp"
#include <glm/glm.hpp>

namespace HateEngine {
    class ObjectUI {
        friend class OpenGL15;
        UUID uuid;

    public:
        enum Type {
            Widget,
            Label
        };
        
        enum PositionMode {
            Pixels,
            Percent
        };
        
    private:
        Type type;

    protected:
        ObjectUI(Type type);
        

    public:
        glm::vec2 position = {0, 0};
        

        UUID getUUID() const;
    };
}
