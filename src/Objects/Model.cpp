#include <HateEngine/Objects/Model.hpp>

using namespace HateEngine;

Model::Model() {
}

Model::Model(const Model& model, bool copy_textures) : Object(model) {
    this->meshes.reserve(model.meshes.size());
    for (const auto m: model.meshes)
        this->meshes.push_back(new Mesh(*m));
    // this->textures = model.textures;

    if (copy_textures) {
        this->textures.reserve(model.textures.size());
        for (const auto& t: model.textures)
            this->textures.push_back(Texture(t, true));
    } else
        this->textures = model.textures;
}

std::vector<Mesh*> Model::getMeshes() const {
    return this->meshes;
}

void Model::setVisible(bool visible) {
    for (const auto m: this->meshes)
        m->setVisible(visible);
}

Model::~Model() {
    for (const auto m: this->meshes) {
        delete m;
    }
}
