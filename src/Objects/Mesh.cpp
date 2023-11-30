#include <Old3DEngine/Objects/Mesh.hpp>
#include <algorithm>
#include <utility>

using namespace Old3DEngine;

Mesh::Mesh() {}

Mesh::Mesh(std::vector<float> vert, std::vector<uint32_t> ind, std::vector<float> norm) {
    this->verticies = std::move(vert);
    this->indicies = std::move(ind);
    this->normals = std::move(norm);
}

// FIXME: DIRTY HACK
Mesh::Mesh(const Mesh &mesh) {
    position = mesh.position;
    rotation_matrix = mesh.rotation_matrix;
    scale = mesh.scale;
    visible = mesh.visible;
    verticies = mesh.verticies;
    indicies = mesh.indicies;
    normals = mesh.normals;
    texture = mesh.texture;
    UV = mesh.UV;
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



void Mesh::setTexture(Texture *tex) {
    this->texture = tex;
}

void Mesh::setUV(std::vector<float> uv) {
    this->UV = uv;
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

Texture* Mesh::getTexture() {
    return this->texture;
}

const std::vector<float>* Mesh::getUV() {
    return &this->UV;
}


