#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include "../Objects/LODMesh.hpp"
#include "../Objects/Object.hpp"
#include "Texture.hpp"

namespace HateEngine {
    class Model : public Object, public Resource {
        friend class GLTFAnimationPlayer;

    protected:
        /*struct LOD {
            float distance = 0;
            std::vector<Mesh*> meshes = {};
        };

        std::vector<LOD> LODs;*/
        std::vector<Texture> textures;

        /*

            struct lod {
                distance: float;
                mesh: Mesh*;
            }

            struct M {
                name: string;
                lods: vector<lod>;
            }

            vector<M> m;        

        */

        std::vector<LODMesh> meshes;

    public:
        Model();
        Model(const Model& model, bool copy_textures = false);

        void addMesh(LODMesh mesh);

        std::vector<LODMesh>* getLODMeshes();

        //size_t getLODCount() const;
        //std::vector<Mesh*>& getLOD(size_t index);

        //void setVisible(bool visible);
        //~Model();
    };
} // namespace HateEngine
