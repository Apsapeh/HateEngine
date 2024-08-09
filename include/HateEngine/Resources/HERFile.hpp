#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <HateEngine/Resources/Texture.hpp>

namespace HateEngine {
    class HERResource {
        friend class HERFile;
        std::vector<uint8_t> data;

    public:
        std::vector<uint8_t>* getRawData();
        Texture asTexture(
                Texture::TexWrap tex_wrap = Texture::Repeat,
                Texture::TexFiltering tex_filtering = Texture::Linear, bool mipmap = true,
                float mipmap_bias = -1.0f, bool autoload = true
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

        HERResource operator[](std::string key);
    };
} // namespace HateEngine
