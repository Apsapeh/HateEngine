//
// Created by Тимофей Кириченко on 30.04.2024.
//

#pragma once

#include <cstdint>
#include <vector>
#include "CollisionShape.hpp"

namespace HateEngine {

    class ConvexShape : public CollisionShape {
    private:
    public:
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        std::vector<reactphysics3d::PolygonVertexArray::PolygonFace> react_faces;
        reactphysics3d::PolygonVertexArray vertexArray;
        ConvexShape(
                std::vector<float> vertices, std::vector<std::vector<uint32_t>> faces_indices,
                glm::vec3 pos, glm::vec3 rot
        );
    };

} // namespace HateEngine
