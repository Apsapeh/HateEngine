#pragma once
#include <cstdint>
#include <vector>
#include "../Resources/Texture.hpp"
#include "Object.hpp"

namespace HateEngine {
    // Object -> Mesh
    class Mesh : public Object {
    protected:
        std::vector<float> verticies;
        std::vector<uint32_t> indicies;
        std::vector<float> normals;
        Texture* texture = nullptr;
        std::vector<float> UV = {};
        float max_light_dist = 0;
        bool is_shaded = true;

    public:
        ~Mesh();
        Mesh();
        Mesh(std::vector<float> vert, std::vector<uint32_t> ind, std::vector<float> norm);
        Mesh(const Mesh& mesh, bool copy_texture = false);

        void setVertices(std::vector<float> vec);
        void setIndicies(std::vector<uint32_t> vec);
        void setNormals(std::vector<float> vec);
        void setTexture(Texture* tex);
        void setUV(std::vector<float> UV);

        void enableCustomMaxLightDist(float dist);
        const float getCustomMaxLightDist() const;
        void disableCustomMaxLightDist();

        void enableLightShading();
        void disableLightShading();
        bool isLightShading() const;

        const std::vector<float>* getVertices() const;
        const std::vector<uint32_t>* getIndicies() const;
        const std::vector<float>* getNormals() const;
        Texture* getTexture() const;
        const std::vector<float>* getUV() const;
        // const std::vector<std::vector<float>>* getTexturesCoords();
    };
} // namespace HateEngine
