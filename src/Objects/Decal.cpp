#include <HateEngine/Objects/Decal.hpp>
#include "HateEngine/Log.hpp"
#include "glm/ext/matrix_transform.hpp"

using namespace HateEngine;

Decal::Decal() {
    mesh.setVisible(false);
    ray.enableCustomDirection(ray_direction);

    std::vector<uint32_t> indicies;
    std::vector<float> normals;
    std::vector<float> UV;
    indicies.reserve(6);

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

    this->mesh.setIndicies(indicies);
    this->mesh.setNormals(normals);
    this->mesh.setUV(UV);

    this->bindObj(&mesh, true, false);
    this->bindObj(&ray, true, false);
}


void Decal::bake() {
    if (this->is_in_position) {
        this->mesh.lookAt(this->getGlobalPosition() + this->custom_normal);
    } else {
        const std::vector<RayCastInfo> hits =
                this->ray.getSortedByDistanceAllCollisions(this->getGlobalPosition());
        if (!hits.empty()) {
            this->mesh.setPosition(
                    hits[0].worldPoint - this->getGlobalPosition() +
                    glm::vec3(0.01f, 0.01f, 0.01f) * hits[0].worldNormal
            );
            mesh.setRotationMatrix(glm::lookAt({0, 0, 0}, -hits[0].worldNormal, glm::vec3(0, 0, 1)));
            mesh.setVisible(true);
        } else {
            mesh.setVisible(false);
        }
    }
}

void Decal::setSize(float width, float height) {
    float x = 0.5f * width;
    float y = 0.5f * height;
    std::vector<float> verticies;

    verticies = {
            -x, -y, 0, x, -y, 0, x, y, 0, -x, y, 0,
    };

    this->mesh.setVertices(verticies);
}

void Decal::setTexture(Texture* texture) {
    this->mesh.setTexture(texture);
}

void Decal::enableLightShading() {
    this->mesh.enableLightShading();
}

void Decal::disableLightShading() {
    this->mesh.disableLightShading();
}

void Decal::setPhysEngine(PhysEngine* physEngine) {
    this->ray.setPhysEngine(physEngine);
}

void Decal::enableInPosition(glm::vec3 normal) {
    this->is_in_position = true;
    this->mesh.setVisible(true);
    this->setNormal(normal);
    this->bake();
}

void Decal::setNormal(glm::vec3 normal) {
    this->custom_normal = glm::normalize(normal);
}

void Decal::disableInPosition() {
    this->is_in_position = false;
    this->mesh.setVisible(false);
}

void Decal::setRayDirection(glm::vec3 direction) {
    this->ray_direction = direction;
    this->ray.enableCustomDirection(direction);
}

void Decal::setRayLength(float length) {
    this->ray.changeLength(length);
}

glm::vec3 Decal::getInPositionNormal() const {
    return this->custom_normal;
}

bool Decal::isInPosition() const {
    return this->is_in_position;
}

glm::vec3 Decal::getRayDirection() const {
    return this->ray_direction;
}

float Decal::getRayLength() const {
    return this->ray.getLength();
}

Texture* Decal::getTexture() const {
    return this->mesh.getTexture();
}

bool Decal::isLightShading() const {
    return this->mesh.isLightShading();
}

Mesh* Decal::getMesh() const {
    return (Mesh*) &this->mesh;
}
