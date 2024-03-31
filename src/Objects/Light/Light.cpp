#include <HateEngine/Objects/Light/Light.hpp>

using namespace HateEngine;

Light::Light(Light::LightTypeEnum type) {
    lightType = type;
    color = {1.0, 1.0, 1.0, 1.0};
}

Light::LightTypeEnum Light::getLightType() const {
    return lightType;
}

std::vector<float> Light::getColor() const {
    return color;
}
