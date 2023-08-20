#pragma once
#include <cstdint>
#include <vector>
#include "Object.hpp"
#include "../Resources/Texture.hpp"

namespace Old3DEngine {
    // Object -> Mesh
    class Mesh : public Object {
    protected:
        std::vector<float> verticies;
        std::vector<uint32_t> indicies;
        std::vector<float> normals;
        std::vector<Texture*> textures;
        std::vector<std::vector<float>> texturesCoords;

    public:
        ~Mesh();

        void setVertices(std::vector<float> vec);
        void setIndicies(std::vector<uint32_t> vec);
        void setNormals(std::vector<float> vec);
        bool addTexture(Texture* tex, std::vector<float> coords);
        bool delTexture(uint32_t n);

        std::vector<float>*     getVertices();
        std::vector<uint32_t>*  getIndicies();
        std::vector<float>*     getNormals();
        const std::vector<Texture*>*  getTextures();
        const std::vector<std::vector<float>>* getTexturesCoords();
    };
}