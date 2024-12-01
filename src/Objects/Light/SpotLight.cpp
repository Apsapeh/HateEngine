#include <HateEngine/Objects/Light/SpotLight.hpp>
#include "HateEngine/Log.hpp"

using namespace HateEngine;

SpotLight::SpotLight() : Light(Light::LightTypeEnum::SpotLight) {
}

void SpotLight::setAngleCutoff(float angleCutoff) {
    if (angleCutoff < 0.0f) {
        HATE_WARNING_F(
                "SpotLight [%llu]: angleCutoff cannot be negative. Setting to 0.",
                this->getUUID().getU64()
        );
        angleCutoff = 0.0f;
    }

    if (angleCutoff > 180.0f) {
        HATE_WARNING_F(
                "SpotLight [%llu]: angleCutoff cannot be greater than 180. Setting to 180.",
                this->getUUID().getU64()
        );
        angleCutoff = 180.0f;
    }

    this->angleCutoff = angleCutoff * 0.5f;
}
void SpotLight::setExponent(float exponent) {
    if (exponent < 0.0f) {
        HATE_WARNING_F(
                "SpotLight [%llu]: exponent cannot be negative. Setting to 0.",
                this->getUUID().getU64()
        );
        exponent = 0.0f;
    }

    if (exponent > 128.0f) {
        HATE_WARNING_F(
                "SpotLight [%llu]: exponent cannot be greater than 128. Setting to 128.",
                this->getUUID().getU64()
        );
        exponent = 128.0f;
    }

    this->exponent = exponent;
}
float SpotLight::getAngleCutoff() const {
    return angleCutoff * 2.0f;
}
float SpotLight::getExponent() const {
    return exponent;
}
