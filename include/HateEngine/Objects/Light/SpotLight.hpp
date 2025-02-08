#pragma once
#include "Light.hpp"

namespace HateEngine {
    class SpotLight : public Light {
        friend class OpenGL_1_3;

    protected:
        float angleCutoff = 22.5f;
        float exponent = 0.0f;

    public:
        SpotLight();

        void setAngleCutoff(float angleCutoff = 45.0f);
        void setExponent(float exponent = 0.0f);

        float getAngleCutoff() const;
        float getExponent() const;
    };
} // namespace HateEngine
