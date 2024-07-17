#pragma once
#include "../Objects/Model.hpp"
#include <string>

namespace HateEngine {
    class ObjMapModel : public Model {

    private:
        void parseObj(std::string data);

    public:
        /**
         * @brief Load model from file (.gltf/.glb)
         *
         * @param str File name
         */
        ObjMapModel(std::string obj_file_name, std::string map_file_name);

        /**
         * @brief Load model from memory as ASCII string (.gltf)
         *
         * @param data ASCII string
         * @param size Data length
         * @param dir Directory
         */
        ObjMapModel(const char* data, uint32_t size, std::string dir);
    };
}

