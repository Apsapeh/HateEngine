#pragma once
#include "../Objects/Model.hpp"
#include <string>

namespace HateEngine {
    class QMapModel : public Model{
    public:
        /**
         * @brief Load model from file (.gltf/.glb)
         *
         * @param str File name
         */
        QMapModel(std::string file_name);

        /**
         * @brief Load model from memory as ASCII string (.gltf)
         *
         * @param data ASCII string
         * @param size Data length
         * @param dir Directory
         */
        QMapModel(const char* data, uint32_t size, std::string dir);
    };
}

