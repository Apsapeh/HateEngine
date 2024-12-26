#pragma once
#include <cstdint>
#include <string>
#include <soloud_wav.h>
#include "Resource.hpp"

namespace HateEngine {
    class Audio : public Resource {
        SoLoud::Wav soloudWav;

    public:
        /**
         * @brief Load model from file (.gltf/.glb)
         *
         * @param str File name
         */
        Audio(std::string file_name);

        /**
         * @brief Load model from memory (.glb)
         *
         * @param data
         */
        Audio(const uint8_t* data, uint32_t size, std::string name);

        ~Audio();

        /**
         * @brief Get the SoLoud Wav object
         *
         * @return SoLoud::Wav
         */
        SoLoud::Wav* GetSoLoudWav();
    };
} // namespace HateEngine
