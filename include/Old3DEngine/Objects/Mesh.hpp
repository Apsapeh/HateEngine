#pragma once
#include <cstdint>
#include <vector>
#include "Object.hpp"
#include "../Utilities/UUID_Generator.hpp"
#include "../Resources/Texture.hpp"

namespace Old3DEngine {
    // Object -> Mesh
    class Mesh : public Object {
    public:
        struct TextureObject {
            Texture* texture;
            std::vector<float> UV;
            UUID_Generator::UUID id;
        };

    private:
        UUID_Generator tex_uuid_generator;


    protected:
        std::vector<float> verticies;
        std::vector<uint32_t> indicies;
        std::vector<float> normals;
        std::vector<TextureObject> textures;

    public:
        ~Mesh();
        Mesh();
        Mesh(const Mesh& mesh);

        void setVertices(std::vector<float> vec);
        void setIndicies(std::vector<uint32_t> vec);
        void setNormals(std::vector<float> vec);
        UUID_Generator::UUID addTexture(Texture* tex, std::vector<float> uv);
        bool delTexture(UUID_Generator::UUID uuid);

        const std::vector<float>*     getVertices();
        const std::vector<uint32_t>*  getIndicies();
        const std::vector<float>*     getNormals();
        const std::vector<TextureObject>*  getTextures();
        //const std::vector<std::vector<float>>* getTexturesCoords();
    };
}