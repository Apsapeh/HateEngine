#pragma once
#include "../Objects/Mesh.hpp"
#include "../Objects/Object.hpp"
#include "../Resources/Texture.hpp"

namespace HateEngine {
    class Model : public Object {
        friend class GLTFAnimationPlayer;

    protected:
        std::vector<Mesh*> meshes;
        std::vector<Texture> textures;

        void setParentPosition(glm::vec3 vec) override;
        void setParentScale(glm::vec3 vec) override;
        void setParentRotationMatrix(const glm::mat4& mat) override;

    public:
        Model();
        Model(const Model& model, bool copy_textures = false);

        std::vector<Mesh*> getMeshes() const;

        void setVisible(bool visible);
        ~Model();
    };
} // namespace HateEngine
