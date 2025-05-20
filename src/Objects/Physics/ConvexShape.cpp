//
// Created by Тимофей Кириченко on 30.04.2024.
//

#include <HateEngine/Objects/Physics/ConvexShape.hpp>
#include <cstdint>
#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "reactphysics3d/collision/PolygonVertexArray.h"

using namespace HateEngine;

ConvexShape::ConvexShape(
        std::vector<float> _vertices, std::vector<std::vector<uint32_t>> faces_indices, glm::vec3 pos,
        glm::vec3 rot
) :
    CollisionShape(Convex, pos, rot),
    vertexArray(
            0, nullptr, 0, nullptr, 0, 0, nullptr,
            reactphysics3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
            reactphysics3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
    ) {
    this->vertices = std::move(_vertices);

    react_faces.reserve(faces_indices.size());

    for (auto& face: faces_indices) {
        react_faces.push_back({(uint32_t) face.size(), (uint32_t) indices.size()});
        indices.insert(indices.end(), face.begin(), face.end());
    }
    faces_indices.clear();


    vertexArray = reactphysics3d::PolygonVertexArray(
            vertices.size() / 3, vertices.data(), 3 * sizeof(float), indices.data(), sizeof(int),
            react_faces.size(), react_faces.data(),
            reactphysics3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
            reactphysics3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
    );
}

// Если переопределить деструктор, то всё падает с сегфолтом
// НЕНАВИЖУ C++
