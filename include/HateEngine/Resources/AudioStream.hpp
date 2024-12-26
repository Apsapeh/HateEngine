#pragma once
#include <cstdint>
#include <string>
#include <soloud_wavstream.h>
#include "Resource.hpp"

namespace HateEngine {
    class AudioStream : public Resource {
        SoLoud::WavStream soloudWavStream;

    public:
        /**
         * @brief Load audio from file (.wav, .ogg, .mp3)
         *
         * @param str File name
         */
        AudioStream(std::string file_name);

        /**
         * @brief Load audio from memory
         *
         * @param data
         */
        AudioStream(const uint8_t* data, uint32_t size, std::string name);

        ~AudioStream();

        /**
         * @brief Get the SoLoud Wav object
         *
         * @return SoLoud::Wav
         */
        SoLoud::WavStream* GetSoLoudWavStream();
    };
} // namespace HateEngine
