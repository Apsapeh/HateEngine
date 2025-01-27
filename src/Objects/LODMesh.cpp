#include <HateEngine/Objects/LODMesh.hpp>
#include <algorithm>
#include <cstdint>
#include <utility>
#include "glm/fwd.hpp"

using namespace HateEngine;

LODMesh::LODMesh() {
}

void LODMesh::addLOD(float distance, Mesh* mesh) {
    this->LODs.push_back({distance, mesh});
}

Mesh* LODMesh::getMeshByPos(glm::vec3 pos) {
    if (this->LODs.size() == 0)
        return nullptr;
    else if (this->LODs.size() == 1)
        return this->LODs[0].mesh;

    for (unsigned long i = 0; i < this->LODs.size() - 1; i++) {
        if (this->LODs[i].mesh->getAABBDistanceToPoint(pos) > this->LODs[i].distance && this->LODs[i + 1].mesh->getAABBDistanceToPoint(pos) < this->LODs[i + 1].distance) {
            return this->LODs[i].mesh;
        }
    }
    
    return this->LODs[this->LODs.size() - 1].mesh;
}