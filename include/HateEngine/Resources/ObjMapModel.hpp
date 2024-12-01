#pragma once
#include <cstddef>
#include <string>
#include <unordered_map>
#include "../Objects/Model.hpp"
#include "HateEngine/Resources/Texture.hpp"

namespace HateEngine {
    class ObjMapModel : public Model {

    private:
        struct Material {
            size_t texture_id;
        };

        struct ObjFace {
            std::vector<int32_t> indices;
            float normal[3] = {0.0f, 0.0f, 0.0f};
            std::vector<int32_t> tex_indices;
        };

        struct ObjObject {
            std::string name;
            std::string material = "";
            std::vector<ObjFace> faces;
        };

        void parseObj(
                std::string data, float lod_dist, float lod_step, class HERFile* her = nullptr
        );
        std::unordered_map<std::string, Material> parseMtlLib(
                std::string data, class HERFile* her = nullptr
        );

        std::vector<Mesh*> generateLod(
                std::vector<glm::vec3> vertices, std::vector<glm::vec2> tex_coords,
                std::vector<ObjObject> objects, float step = 1.0f
        );

        std::string obj_file_path;
        std::unordered_map<std::string, Material> materials;

    public:
        /**
         * @brief Load model from file (.gltf/.glb)
         *
         * @param str File name
         */
        ObjMapModel(
                std::string obj_file_name, std::string map_file_name, float lod_dist = 15,
                float lod_step = 1.0
        );

        /**
         * @brief Load model from memory as ASCII string (.gltf)
         *
         * @param data ASCII string
         * @param size Data length
         * @param dir Directory
         */
        ObjMapModel(

                std::string obj_file_data, std::string map_file_data, class HERFile* her,
                float lod_dist = 15, float lod_step = 1.0
        );
    };
} // namespace HateEngine
