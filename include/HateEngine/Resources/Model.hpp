#pragma once
#include "../Objects/LODMesh.hpp"
#include "../Objects/Object.hpp"
#include "Texture.hpp"

namespace HateEngine {
    class Model : public Object, public Resource, public Renderable3DInterface {
        friend class GLTFAnimationPlayer;

    protected:
        std::vector<Texture> textures;

        std::vector<LODMesh> meshes;

    public:
        Model();
        Model(const Model& model, bool copy_textures = false);

        void addMesh(LODMesh mesh);
        std::vector<LODMesh>* getLODMeshes();

        void render(RenderInterface* renderer) override;
    };
} // namespace HateEngine
