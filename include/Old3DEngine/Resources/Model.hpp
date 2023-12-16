#pragma once
#include "../Objects/Object.hpp"
#include "../Objects/Mesh.hpp"
#include "Texture.hpp"

namespace Old3DEngine {
    class Model : public Object {
    protected:
        std::vector<Mesh*> meshes;
        std::vector<Texture> textures;

    public:
        std::vector<Mesh*> getMeshes();
        ~Model();
    };
}
