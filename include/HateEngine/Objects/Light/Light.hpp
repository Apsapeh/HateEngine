#pragma once
#include <vector>
#include "../Object.hpp"
#include "glm/ext/vector_float4.hpp"

namespace HateEngine {
    class Light : public Object {
        friend class OpenGL_1_3;

    public:
        enum LightTypeEnum { DirectionalLight, OmniLight, SpotLight };

    protected:
        glm::vec4 color;
        glm::vec3 attenuation = {0.5f, 0.5f, 0.9f};

        Light(LightTypeEnum type);

    public:
        void setColor(glm::vec4 color);
        // void setDifuuse(float dif);
        // void setSpecular(float spec);
        // void setAmbient(float amb);
        void setAttenuation(float constant, float linear, float quadratic);
        void setConstantAttenuation(float constant);
        void setLinearAttenuation(float linear);
        void setQuadraticAttenuation(float quadratic);


        LightTypeEnum getLightType() const;
        glm::vec4 getColor() const;
        glm::vec3 getAttenuation() const;
        float getConstantAttenuation() const;
        float getLinearAttenuation() const;
        float getQuadraticAttenuation() const;

    protected:
        LightTypeEnum lightType;

    public:
    };
} // namespace HateEngine
