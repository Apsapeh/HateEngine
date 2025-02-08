#include <HateEngine/Resources/Model.hpp>

using namespace HateEngine;

Model::Model() {
}

Model::Model(const Model& model, bool copy_textures) : Object(model) {
}

void Model::addMesh(LODMesh mesh) {
    this->meshes.push_back(std::move(mesh));
}

std::vector<LODMesh>* Model::getLODMeshes() {
    return &this->meshes;
}


void Model::render(RenderInterface* renderer) {
    for (auto& mesh: this->meshes) {
        mesh.render(renderer);
    }
}
