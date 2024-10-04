#pragma once
#include <cstdint>
#include <string>
#include "../Objects/Model.hpp"

namespace HateEngine {
    class GLTFModel : public Model {
    public:
        /**
         * @brief Load model from file (.gltf/.glb)
         *
         * @param str File name
         */
        GLTFModel(std::string file_name);

        /**
         * @brief Load model from memory as ASCII string (.gltf)
         *
         * @param data ASCII string
         * @param size Data length
         * @param dir Directory
         */
        GLTFModel(const char* data, uint32_t size, std::string dir);

        /**
         * @brief Load model from memory (.glb)
         *
         * @param data
         */
        GLTFModel(const uint8_t* data, uint32_t size, std::string dir = "");
    };
} // namespace HateEngine
