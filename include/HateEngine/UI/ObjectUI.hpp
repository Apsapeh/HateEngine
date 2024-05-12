//
// Created by Тимофей Кириченко on 12.05.2024.
//

#pragma once

#include "../Utilities/UUID.hpp"
#include <glm/glm.hpp>

namespace HateEngine {
    class ObjectUI {
        UUID uuid;

    public:
        glm::vec2 position = {0, 0};

        UUID getUUID() const;
    };
}
