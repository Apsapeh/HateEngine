#include <HateEngine/Objects/Light/Light.hpp>

using namespace HateEngine;

Light::Light(Light::LightTypeEnum type) {
    lightType = type;
    color = {1.0, 1.0, 1.0, 1.0};
}


void Light::setColor(glm::vec4 color) {
    this->color = color;
}

void Light::setAttenuation(float constant, float linear, float quadratic) {
    attenuation = {constant, linear, quadratic};
}

void Light::setConstantAttenuation(float constant) {
    attenuation.x = constant;
}

void Light::setLinearAttenuation(float linear) {
    attenuation.y = linear;
}

void Light::setQuadraticAttenuation(float quadratic) {
    attenuation.z = quadratic;
}


Light::LightTypeEnum Light::getLightType() const {
    return lightType;
}

glm::vec4 Light::getColor() const {
    return color;
}

glm::vec3 Light::getAttenuation() const {
    return attenuation;
}

float Light::getConstantAttenuation() const {
    return attenuation.x;
}

float Light::getLinearAttenuation() const {
    return attenuation.y;
}

float Light::getQuadraticAttenuation() const {
    return attenuation.z;
}
