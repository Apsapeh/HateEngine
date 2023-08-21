#include <Old3DEngine/Objects/Mesh.hpp>
#include <algorithm>

using namespace Old3DEngine;
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

bool Mesh::addTexture(Texture *tex, std::vector<float> coords) {
    if (this->textures.size() > 0)
        return false;

    this->textures.push_back(tex);
    this->texturesCoords.push_back(coords);
    return true;
}

bool Mesh::delTexture(uint32_t n) {
    if (n > textures.size())
        return false;

    textures.erase(textures.begin() + n);
    texturesCoords.erase(texturesCoords.begin() + n);
    return true;
}



std::vector<float>* Mesh::getVertices() {
    return &verticies;
}

std::vector<uint32_t>* Mesh::getIndicies() {
    return &indicies;
}

std::vector<float>* Mesh::getNormals() {
    return &normals;
}

const std::vector<Texture *> *Mesh::getTextures() {
    return &textures;
}

const std::vector<std::vector<float>> *Mesh::getTexturesCoords() {
    return &texturesCoords;
}

