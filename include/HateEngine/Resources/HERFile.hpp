#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "HateEngine/AudioBus.hpp"
#include "HateEngine/Resources/HENFile.hpp"
#include "HateEngine/Resources/ObjMapModel.hpp"
#include "Texture.hpp"
#include "GLTFModel.hpp"
#include "Audio.hpp"
#include "AudioStream.hpp"
#include "../Objects/AudioPlayer.hpp"

namespace HateEngine {
    class HERResource {
        friend class HERFile;
        std::vector<uint8_t> data = {};

    public:
        std::vector<uint8_t>* getRawData();
        std::string asString();
        Texture asTexture(
                Texture::TexWrap tex_wrap = Texture::Repeat,
                Texture::TexFiltering tex_filtering = Texture::Linear, bool mipmap = true,
                float mipmap_bias = -1.0f, bool autoload = true
        );
        GLTFModel asGLBModel();
        HENFile asHENFile();
        Audio asAudio();
        AudioStream asAudioStream();
        AudioPlayer asAudioPlayer(
                AudioBus* bus = nullptr, AudioPlayer::Type type = AudioPlayer::Audio2D
        );
        AudioPlayer asAudioPlayerStream(
                AudioBus* bus = nullptr, AudioPlayer::Type type = AudioPlayer::Audio2D
        );
    };

    class HERFile {
    private:
        struct HERResourceRaw {
            uint64_t originalSize;
            uint64_t alignedSize;
            uint64_t offset;
        };
        std::string path;
        std::string password;
        uint64_t dataPointer;

        std::unordered_map<std::string, HERResourceRaw> resources;

    public:
        HERFile(std::string path, std::string password);

        ObjMapModel loadObjMap(
                std::string obj_file_name, std::string map_file_name, float grid_size = 16.0,
                bool generate_collision = true, float lod_dist = 15, float lod_step = 1.0
        );

        HERResource operator[](std::string key);
    };
} // namespace HateEngine
