#include <HateEngine/Objects/Light/Light.hpp>
#include <HateEngine/Log.hpp>
#include <cstdint>

using namespace HateEngine;

Light::Light(Light::LightTypeEnum type) {
    this->lightType = type;
    this->color = {1.0, 1.0, 1.0, 1.0};
}


void Light::setRadius(float radius) {
    this->radius = radius;
}

void Light::setColor(glm::vec4 color) {
    this->color = color;
}

void Light::setAttenuation(float constant, float linear, float quadratic) {
    this->attenuation = {constant, linear, quadratic};
}

void Light::setConstantAttenuation(float constant) {
    this->attenuation.x = constant;
}

void Light::setLinearAttenuation(float linear) {
    this->attenuation.y = linear;
}

void Light::setQuadraticAttenuation(float quadratic) {
    this->attenuation.z = quadratic;
}

void Light::setMaskBit(uint8_t bit, bool state) {
    this->mask.set(bit, state);
    this->onMaskChanged.emit(this);
}

bool Light::getMaskBit(uint8_t bit) const {
    return this->mask.get(bit);
}

Light::LightTypeEnum Light::getLightType() const {
    return this->lightType;
}

float Light::getRadius() const {
    return this->radius;
}

glm::vec4 Light::getColor() const {
    return this->color;
}

glm::vec3 Light::getAttenuation() const {
    return this->attenuation;
}

float Light::getConstantAttenuation() const {
    return this->attenuation.x;
}

float Light::getLinearAttenuation() const {
    return this->attenuation.y;
}

float Light::getQuadraticAttenuation() const {
    return this->attenuation.z;
}
