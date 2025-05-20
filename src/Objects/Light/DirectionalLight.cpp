#include <HateEngine/Objects/Light/DirectionalLight.hpp>

using namespace HateEngine;

DirectionalLight::DirectionalLight() : Light(LightTypeEnum::DirectionalLight) {
    this->radius = 0.0f;
};
