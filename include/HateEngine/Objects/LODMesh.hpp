#pragma once
#include "Mesh.hpp"

namespace HateEngine {
    // Object -> Mesh
    class LODMesh : public Object, public Renderable3DInterface {
    private:
        struct LOD {
            float distance;
            Mesh* mesh;
        };

        std::vector<LOD> LODs;

    public:
        LODMesh();
        void addLOD(float distance, Mesh* mesh);
        Mesh* getMeshByPos(glm::vec3 pos);

        void render(RenderInterface* renderer) override;
    };
} // namespace HateEngine
