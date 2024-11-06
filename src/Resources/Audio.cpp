#include <HateEngine/Resources/Audio.hpp>
#include <HateEngine/Log.hpp>
#include "soloud_error.h"

using namespace HateEngine;

Audio::Audio(std::string filename) {
    // soloudWav.
    if (this->soloudWav.load(filename.c_str()) != SoLoud::SO_NO_ERROR) {
        HATE_ERROR_F("Failed to load audio file: %s", filename.c_str());
    }
}

Audio::Audio(const uint8_t* data, uint32_t size, std::string name) {
    if (this->soloudWav.loadMem(data, size, true) != SoLoud::SO_NO_ERROR) {
        HATE_ERROR_F("Failed to load audio resource: %s", name.c_str());
    }
}

Audio::~Audio() {
    this->soloudWav.stop();
}


SoLoud::Wav* Audio::GetSoLoudWav() {
    return &this->soloudWav;
}
