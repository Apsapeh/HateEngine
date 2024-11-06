#include <HateEngine/Resources/AudioStream.hpp>
#include <HateEngine/Log.hpp>
#include "soloud_error.h"

using namespace HateEngine;

AudioStream::AudioStream(std::string filename) {
    // soloudWav.
    if (this->soloudWavStream.load(filename.c_str()) != SoLoud::SO_NO_ERROR) {
        HATE_ERROR_F("Failed to load audio file: %s", filename.c_str());
    }
}

AudioStream::AudioStream(const uint8_t* data, uint32_t size, std::string name) {
    if (this->soloudWavStream.loadMem(data, size, true) != SoLoud::SO_NO_ERROR) {
        HATE_ERROR_F("Failed to load audio resource: %s", name.c_str());
    }
}

AudioStream::~AudioStream() {
    this->soloudWavStream.stop();
}


SoLoud::WavStream* AudioStream::GetSoLoudWavStream() {
    return &this->soloudWavStream;
}
