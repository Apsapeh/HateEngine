#include <HateEngine/Resources/Model.hpp>
#include <cstddef>
#include "HateEngine/Objects/Mesh.hpp"

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



/*size_t Model::getLODCount() const {
    return LODs.size();
}

std::vector<Mesh*>& Model::getLOD(size_t index) {
    return LODs[index].meshes;
}*/

/*void Model::setVisible(bool visible) {
    for (const auto l: this->LODs)
        for (const auto m: l.meshes)
            m->setVisible(visible);
}

Model::~Model() {
    for (const auto l: this->LODs)
        for (const auto m: l.meshes)
            delete m;
}*/
