#pragma once
#include <vector>
#include "../Object.hpp"
#include "../../Types/Bitset.hpp"
#include "../../Utilities/Signal.hpp"

namespace HateEngine {
    class Light : public Object {
        friend class OpenGL_1_3;

    public:
        enum LightTypeEnum { DirectionalLight, OmniLight, SpotLight };

    protected:
        LightTypeEnum lightType;
        /**
         * @brief If the AABB of the object is farther than the specified radius, the light source will
         * not affect the object
         *
         */
        float radius = 1.0f;
        glm::vec4 color;
        glm::vec3 attenuation = {0.5f, 0.5f, 0.9f};
        Bitset<uint8_t> mask = 1;

        Light(LightTypeEnum type);

    public:
        Signal<Light*> onMaskChanged;

        /**
         * @brief If the AABB of the object is farther than the specified radius, the light source will
         * not affect the object
         *
         */
        void setRadius(float radius);
        void setColor(glm::vec4 color);
        // void setDifuuse(float dif);
        // void setSpecular(float spec);
        // void setAmbient(float amb);
        void setAttenuation(float constant, float linear, float quadratic);
        void setConstantAttenuation(float constant);
        void setLinearAttenuation(float linear);
        void setQuadraticAttenuation(float quadratic);

        void setMaskBit(uint8_t bit, bool state);
        bool getMaskBit(uint8_t bit) const;

        LightTypeEnum getLightType() const;
        /**
         * @brief If the AABB of the object is farther than the specified radius, the light source will
         * not affect the object
         *
         */
        float getRadius() const;
        glm::vec4 getColor() const;
        glm::vec3 getAttenuation() const;
        float getConstantAttenuation() const;
        float getLinearAttenuation() const;
        float getQuadraticAttenuation() const;
    };
} // namespace HateEngine
