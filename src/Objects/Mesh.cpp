#include <HateEngine/Objects/Mesh.hpp>
#include <algorithm>
#include <cstdint>
#include <utility>
#include "HateEngine/Render/RenderInterface.hpp"
#include "glm/fwd.hpp"

using namespace HateEngine;

Mesh::Mesh() {
}

Mesh::Mesh(std::vector<float> vert, std::vector<uint32_t> ind, std::vector<float> norm) {
    this->verticies = std::move(vert);
    this->indicies = std::move(ind);
    this->normals = std::move(norm);

    updateCenterMaxSize();
    updateAABB();
}

Mesh::Mesh(const Mesh& mesh, bool copy_texture) {
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
    name = mesh.name;

    updateCenterMaxSize();
    updateAABB();

    if (copy_texture and mesh.texture != nullptr)
        texture = new Texture(*mesh.texture, true);
}

Mesh::~Mesh() {
}

void Mesh::updateCenterMaxSize() {
    if (verticies.empty())
        return;

    glm::vec3 min = {verticies[0], verticies[1], verticies[2]};
    glm::vec3 max = {verticies[0], verticies[1], verticies[2]};
    for (uint32_t i = 0; i < verticies.size(); i += 3) {
        min.x = std::min(min.x, verticies[i]);
        min.y = std::min(min.y, verticies[i + 1]);
        min.z = std::min(min.z, verticies[i + 2]);

        max.x = std::max(max.x, verticies[i]);
        max.y = std::max(max.y, verticies[i + 1]);
        max.z = std::max(max.z, verticies[i + 2]);
    }

    this->center_max_size = glm::length(max - min) / 2;
}

void Mesh::updateAABB() {
    if (verticies.empty())
        return;

    std::vector<float> verticies_clone = verticies;
    glm::vec3* verticies_vec = (glm::vec3*) verticies_clone.data();

    glm::mat4 grot = getGlobalRotationMatrix();
    for (uint32_t i = 0; i < verticies.size() / 3; i++) {
        verticies_vec[i] = grot * glm::vec4(verticies_vec[i], 1.0f);
    }

    glm::vec3 min = verticies_vec[0];
    glm::vec3 max = verticies_vec[0];
    for (uint32_t i = 0; i < verticies.size() / 3; i++) {
        min.x = std::min(min.x, verticies_vec[i].x);
        min.y = std::min(min.y, verticies_vec[i].y);
        min.z = std::min(min.z, verticies_vec[i].z);

        max.x = std::max(max.x, verticies_vec[i].x);
        max.y = std::max(max.y, verticies_vec[i].y);
        max.z = std::max(max.z, verticies_vec[i].z);
    }

    this->AABB_min = min;
    this->AABB_max = max;
}

void Mesh::setRotationMatrixRaw(const glm::mat4& mat) {
    Object::setRotationMatrixRaw(mat);
    updateAABB();
}


void Mesh::setVertices(std::vector<float> vec) {
    this->verticies = std::move(vec);
    updateCenterMaxSize();
    updateAABB();
}

void Mesh::setIndicies(std::vector<uint32_t> vec) {
    this->indicies = std::move(vec);
}

void Mesh::setNormals(std::vector<float> vec) {
    this->normals = std::move(vec);
}

void Mesh::setTexture(Texture* tex) {
    this->texture = tex;
}

void Mesh::setLightTexture(Texture* tex) {
    this->light_texture = tex;
}

void Mesh::setUV(std::vector<float> uv) {
    this->UV = std::move(uv);
}

void Mesh::setLightUV(std::vector<float> uv) {
    this->light_UV = std::move(uv);
}

void Mesh::setColors(std::vector<float> colors, uint8_t channels) {
    this->colors = std::move(colors);
    this->color_channels = channels;
    this->is_color_enabled = true;
}

void Mesh::setColor(glm::vec4 color) {
    this->colors.clear();
    this->colors.reserve(this->verticies.size() / 3 * this->color_channels);
    for (uint32_t i = 0; i < this->verticies.size() / 3; i++) {
        this->colors.push_back(color.r);
        this->colors.push_back(color.g);
        this->colors.push_back(color.b);
        this->colors.push_back(color.a);
    }
    this->color_channels = 4;
    this->is_color_enabled = true;
}

void Mesh::setColor(glm::vec3 color) {
    this->colors.clear();
    this->colors.reserve(this->verticies.size());
    for (uint32_t i = 0; i < this->verticies.size() / 3; i++) {
        this->colors.push_back(color.r);
        this->colors.push_back(color.g);
        this->colors.push_back(color.b);
    }
    this->color_channels = 3;
    this->is_color_enabled = true;
}

void Mesh::setCorrectTransparency(bool correct) {
    this->correct_transparency = correct;
}

bool Mesh::getCorrectTransparency() const {
    return this->correct_transparency;
}

void Mesh::setFaceCulling(bool cull) {
    this->face_culling = cull;
}

bool Mesh::getFaceCulling() const {
    return this->face_culling;
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

void Mesh::enableColor() {
    this->is_color_enabled = true;
}
void Mesh::disableColor() {
    this->is_color_enabled = false;
}
bool Mesh::isColorEnabled() const {
    return this->is_color_enabled;
}

float Mesh::getAABBRadius() const {
    return this->center_max_size;
}

glm::vec3 Mesh::getAABBMin() const {
    return this->AABB_min;
}

glm::vec3 Mesh::getAABBMax() const {
    return this->AABB_max;
}

float Mesh::getAABBDistanceToPoint(glm::vec3 point) const {
    glm::vec3 global_pos = Object::getGlobalPosition();
    glm::vec3 clamped = glm::clamp(point, AABB_min + global_pos, AABB_max + global_pos);
    glm::vec3 delta = point - clamped;
    return glm::length(delta);
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

Texture* Mesh::getLightTexture() const {
    return this->light_texture;
}

const std::vector<float>* Mesh::getUV() const {
    return &this->UV;
}

const std::vector<float>* Mesh::getLightUV() const {
    return &this->light_UV;
}

const std::vector<float>* Mesh::getColors() const {
    return &this->colors;
}

const uint8_t Mesh::getColorChannels() const {
    return this->color_channels;
}


std::vector<float>* Mesh::getVerticesMut() {
    return &verticies;
}
std::vector<uint32_t>* Mesh::getIndiciesMut() {
    return &indicies;
}
std::vector<float>* Mesh::getNormalsMut() {
    return &normals;
}


/* RENDER */
void Mesh::render(class RenderInterface* renderer) {
    if (this->getGlobalVisible())
        renderer->renderMesh(this);
}
