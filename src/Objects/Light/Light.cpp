#include <Old3DEngine/Objects/Light/Light.hpp>

using namespace Old3DEngine;

Light::Light(Light::LightTypeEnum type) {
    lightType = type;
    color = {1.0, 1.0, 1.0, 1.0};
}

Light::LightTypeEnum Light::getLightType() {
    return lightType;
}

std::vector<float> Light::getColor() {
    return color;
}
