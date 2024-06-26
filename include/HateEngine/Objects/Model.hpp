#pragma once
#include "../Objects/Object.hpp"
#include "../Objects/Mesh.hpp"
#include "../Resources/Texture.hpp"

namespace HateEngine {
    class Model : public Object {
    protected:
        std::vector<Mesh*> meshes;
        std::vector<Texture> textures;

    public:
        Model();
        Model(const Model& model, bool copy_textures=false);

        std::vector<Mesh*> getMeshes() const;
        ~Model();
    };
}
