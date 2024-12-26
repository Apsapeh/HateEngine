#include <HateEngine/Resources/Model.hpp>
#include <cstddef>
#include "HateEngine/Objects/Mesh.hpp"

using namespace HateEngine;

Model::Model() {
    this->LODs.reserve(1);
}

Model::Model(const Model& model, bool copy_textures) : Object(model) {
    /*this->meshes.reserve(model.meshes.size());
    for (const auto m: model.meshes)
        this->meshes.push_back(new Mesh(*m));*/
    // this->textures = model.textures;
    if (not model.is_loaded)
        return;

    this->LODs.reserve(model.LODs.size());
    for (const auto l: model.LODs) {
        LOD lod;
        lod.distance = l.distance;
        this->LODs.push_back(lod);
        for (const auto m: l.meshes)
            this->LODs.back().meshes.push_back(new Mesh(*m));
    }

    if (copy_textures) {
        this->textures.reserve(model.textures.size());
        for (const auto& t: model.textures)
            this->textures.push_back(Texture(t, true));
    } else
        this->textures = model.textures;

    is_loaded = true;
}

void Model::addLOD(float distance, std::vector<Mesh*> meshes) {
    LOD lod;
    lod.distance = distance;
    lod.meshes = meshes;
    this->LODs.push_back(lod);
}

std::vector<Mesh*> Model::getMeshes(glm::vec3 camera_pos) const {
    if (LODs.size() == 1) {
        return LODs[0].meshes;
    } else {
        // Get AABB from LOD0
        std::vector<Mesh*> result;

        size_t i = 0;
        for (const auto m: LODs[0].meshes) {
            // float d = glm::distance(m->getGlobalPosition(), camera_pos) - m->getAABBRadius();
            float d = m->getAABBDistanceToPoint(camera_pos);

            if (d > LODs[1].distance) {
                result.push_back(LODs[1].meshes[i]);
            } else {
                result.push_back(LODs[0].meshes[i]);
            }
            ++i;
        }

        // glm::vec3 min = {lod0_m0->verticies[0], lod0_m0->verticies[1], lod0_m0->verticies[2]};


        return result;
    }
}

size_t Model::getLODCount() const {
    return LODs.size();
}

std::vector<Mesh*>& Model::getLOD(size_t index) {
    return LODs[index].meshes;
}

void Model::setVisible(bool visible) {
    for (const auto l: this->LODs)
        for (const auto m: l.meshes)
            m->setVisible(visible);
}

Model::~Model() {
    for (const auto l: this->LODs)
        for (const auto m: l.meshes)
            delete m;

    for (auto& t: this->textures) {
        t.Unload();
    }
}
