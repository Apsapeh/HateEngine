#include <HateEngine/Objects/Mesh.hpp>
#include <algorithm>
#include <utility>

using namespace HateEngine;

Mesh::Mesh() {}

Mesh::Mesh(std::vector<float> vert, std::vector<uint32_t> ind, std::vector<float> norm) {
    this->verticies = std::move(vert);
    this->indicies = std::move(ind);
    this->normals = std::move(norm);
}

// FIXME: DIRTY HACK
Mesh::Mesh(const Mesh &mesh, bool copy_texture) {
    parent_position = mesh.parent_position;
    parent_rotation_matrix = mesh.parent_rotation_matrix;
    parent_scale = mesh.parent_scale;
    position = mesh.position;
    rotation_matrix = mesh.rotation_matrix;
    scale = mesh.scale;
    visible = mesh.visible;
    verticies = mesh.verticies;
    indicies = mesh.indicies;
    normals = mesh.normals;
    texture = mesh.texture;
    UV = mesh.UV;

    if (copy_texture and mesh.texture != nullptr)
        texture = new Texture(*mesh.texture, true);
}

Mesh::~Mesh() {

}

void Mesh::setVertices(std::vector<float> vec) {
    this->verticies = std::move(vec);
}

void Mesh::setIndicies(std::vector<uint32_t> vec) {
    this->indicies = std::move(vec);
}

void Mesh::setNormals(std::vector<float> vec) {
    this->normals = std::move(vec);
}



void Mesh::setTexture(Texture *tex) {
    this->texture = tex;
}

void Mesh::setUV(std::vector<float> uv) {
    this->UV = std::move(uv);
}


void Mesh::enableCustomMaxLightDist(float dist) {
    this->max_light_dist = dist;
}
const float Mesh::getCustomMaxLightDist() const {
    return this->max_light_dist;
}
void Mesh::disableCustomMaxLightDist() {
    this->max_light_dist = 0;
}

void Mesh::enableLightShading() {
    this->is_shaded = true;
}
void Mesh::disableLightShading() {
    this->is_shaded = false;
}
bool Mesh::isLightShading() const {
    return this->is_shaded;
}

const std::vector<float>* Mesh::getVertices() const {
    return &verticies;
}

const std::vector<uint32_t>* Mesh::getIndicies() const {
    return &indicies;
}

const std::vector<float>* Mesh::getNormals() const {
    return &normals;
}

Texture* Mesh::getTexture() const {
    return this->texture;
}

const std::vector<float>* Mesh::getUV() const {
    return &this->UV;
}


