#pragma once
#include <cstddef>
#include <cstdint>
#include "../Objects/Mesh.hpp"
#include "../Objects/Object.hpp"
#include "Texture.hpp"

namespace HateEngine {
    class Model : public Object, public Resource {
        friend class GLTFAnimationPlayer;

    protected:
        struct LOD {
            float distance = 0;
            std::vector<Mesh*> meshes = {};
        };

        std::vector<LOD> LODs;
        std::vector<Texture> textures;

    public:
        Model();
        Model(const Model& model, bool copy_textures = false);

        void addLOD(float distance, std::vector<Mesh*> meshes);

        std::vector<Mesh*> getMeshes(glm::vec3 camera_pos) const;

        size_t getLODCount() const;
        std::vector<Mesh*>& getLOD(size_t index);

        void setVisible(bool visible);
        ~Model();
    };
} // namespace HateEngine
