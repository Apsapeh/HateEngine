#pragma once
#include "../Object.hpp"
#include <vector>

namespace HateEngine {
    class Light : public Object {
    public:
        enum LightTypeEnum {
            DirectionalLight,
            OmniLight,
            SpotLight
        };

        Light(LightTypeEnum type);
        LightTypeEnum getLightType() const;
        std::vector<float> getColor() const;
    protected:
        LightTypeEnum lightType;
        bool shadowsEnabled = true;
        std::vector<float> color;
    };
}
