#pragma once
#include "Mesh.hpp"

namespace HateEngine {
    // Object -> Mesh
    class LODMesh : public Object {
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
    };
} // namespace HateEngine
