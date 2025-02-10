#include <HateEngine/Objects/BillboardMesh.hpp>
#include <cstdint>
#include <vector>

using namespace HateEngine;

BillboardMesh::BillboardMesh() {
    std::vector<float> verticies;
    std::vector<uint32_t> indicies;
    std::vector<float> normals;
    std::vector<float> UV;
    verticies.reserve(24);
    indicies.reserve(36);

    setSize(1, 1);

    indicies = {
            0, 1, 2, 2, 3, 0,
    };

    normals = {
            0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    };

    UV = {
            0, 1, 1, 1, 1, 0, 0, 0,
    };

    this->setIndicies(indicies);
    this->setNormals(normals);
    this->setUV(UV);
}

void BillboardMesh::setSize(float width, float height) {
    float x = 0.5f * width;
    float y = 0.5f * height;
    std::vector<float> verticies;

    verticies = {
            -x, -y, 0, x, -y, 0, x, y, 0, -x, y, 0,
    };

    float tex_coord_x = 0;
    float tex_coord_y = 0;
    this->setVertices(verticies);
}

void BillboardMesh::setTarget(Object* obj) {
    this->target = obj;
}

Object* BillboardMesh::getTarget() {
    return this->target;
}

void BillboardMesh::_fixedProcess(class Engine* engine, double delta) {
    if (this->target != nullptr) {
        this->lookAt(this->target->getGlobalPosition());
    }
}
