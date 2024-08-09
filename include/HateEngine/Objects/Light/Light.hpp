#pragma once
#include <vector>
#include "../Object.hpp"

namespace HateEngine {
    class Light : public Object {
    public:
        enum LightTypeEnum { DirectionalLight, OmniLight, SpotLight };

        Light(LightTypeEnum type);
        LightTypeEnum getLightType() const;
        std::vector<float> getColor() const;

    protected:
        LightTypeEnum lightType;

    public:
        std::vector<float> color;
    };
} // namespace HateEngine
