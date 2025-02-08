#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Resource.hpp"

namespace HateEngine {
    class OpenGL_1_3;

    class Texture : public Resource {
        friend OpenGL_1_3;

    private:
        std::vector<uint8_t> data = {};
        int width = 0;
        int height = 0;
        int textureFormat = TexType::RGB;
        std::string fileName;
        int texWrap;
        int texFiltering;
        int texMipMapFiltering;
        float MipMapLodBias = -1.0f;
        bool texMipMapEnabled = true;
        bool autoload;
        uint32_t textureGL_ID = 0;
        bool is_gpu_loaded = false;

        void (*API_unloader)(Texture*);

        bool loadFromFile();

    public:
        enum TexType { RGB, RGBA };
        enum TexWrap { Clamp, Repeat, ClampToEdge, ClampToBorder };
        enum TexFiltering { Nearest, Linear };

        Texture(const Texture& texture, bool copy_tex_data = false);

        Texture(std::string file_name, TexWrap tex_wrap = Repeat,
                TexFiltering tex_filtering = Linear, bool mipmap = true, float mipmap_bias = -1.0f,
                bool autoload = true);

        Texture(std::vector<uint8_t> data, int width, int height, TexType tex_type,
                TexWrap tex_wrap = Repeat, TexFiltering tex_filtering = Linear, bool mipmap = true,
                float mipmap_bias = -1.0f, bool autoload = true);

        // Load packed
        Texture(std::vector<uint8_t> data, TexWrap tex_wrap = Repeat,
                TexFiltering tex_filtering = Linear, bool mipmap = true, float mipmap_bias = -1.0f);
        ~Texture();
        bool Load(
                void (*API_loader)(Texture* texture_ptr), void (*API_unloader)(Texture* texture_ptr)
        );
        void Unload();
        uint32_t getTextureID() const;

        int getWidth() const;
        int getHeight() const;

        void setAutoload(bool value);

        bool isGPULoaded();
    };
} // namespace HateEngine
