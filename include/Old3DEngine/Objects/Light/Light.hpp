#pragma once
#include "../Object.hpp"
#include <vector>

namespace Old3DEngine {
    class Light : public Object {
    public:
        enum LightTypeEnum {
            DirectionalLight,
            OmniLight,
            SpotLight
        };

        Light(LightTypeEnum type);
        LightTypeEnum getType();
        std::vector<float> getColor();
    protected:
        LightTypeEnum lightType;
        bool shadowsEnabled = true;
        std::vector<float> color;
    };
}
