#include <Old3DEngine/Objects/Mesh.hpp>
#include <algorithm>

using namespace Old3DEngine;

Mesh::Mesh() {}

// FIXME: DIRTY HACK
Mesh::Mesh(const Mesh &mesh) {
    position = mesh.position;
    rotation_matrix = mesh.rotation_matrix;
    scale = mesh.scale;
    visible = mesh.visible;
    verticies = mesh.verticies;
    indicies = mesh.indicies;
    normals = mesh.normals;
    textures = mesh.textures;
}

Mesh::~Mesh() {

}

void Mesh::setVertices(std::vector<float> vec) {
    this->verticies = vec;
}

void Mesh::setIndicies(std::vector<uint32_t> vec) {
    this->indicies = vec;
}

void Mesh::setNormals(std::vector<float> vec) {
    this->normals = vec;
}



UUID_Generator::UUID Mesh::addTexture(Texture *tex, std::vector<float> uv) {
    UUID_Generator::UUID uuid = tex_uuid_generator.gen();
    /*if (this->textures.size() > 0)
        return false;*/
    this->textures.push_back({tex, uv, uuid});
    return uuid;
}

bool Mesh::delTexture(UUID_Generator::UUID uuid) {
    std::vector<TextureObject>::iterator iter = std::find_if(
            textures.begin(), textures.end(),
            [&uuid] (TextureObject &obj) -> bool {return obj.id == uuid;}
    );
    if (iter != textures.end() and iter->id == uuid) {
        textures.erase(iter);
        return true;
    }
    return false;
}

const std::vector<float>* Mesh::getVertices() {
    return &verticies;
}

const std::vector<uint32_t>* Mesh::getIndicies() {
    return &indicies;
}

const std::vector<float>* Mesh::getNormals() {
    return &normals;
}

const std::vector<Mesh::TextureObject> *Mesh::getTextures() {
    return &textures;
}


