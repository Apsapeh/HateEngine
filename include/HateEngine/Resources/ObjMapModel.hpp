#pragma once
#include <string>
#include "../Objects/Model.hpp"

namespace HateEngine {
    class ObjMapModel : public Model {

    private:
        void parseObj(std::string data, float lod_dist, float lod_step);

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
        ObjMapModel(const char* data, uint32_t size, std::string dir);
    };
} // namespace HateEngine
