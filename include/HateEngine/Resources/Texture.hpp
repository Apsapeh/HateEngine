#pragma once
#include <string>
#include <vector>

namespace HateEngine {
    class OpenGL15;

    class Texture {
        friend OpenGL15;
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
        bool autoload;
        uint32_t textureGL_ID = 0;
        bool is_loaded = false;

        void (*API_unloader)(Texture*);

        bool loadFromFile();

    public:
        enum TexType {
            RGB = 0x1907,
            RGBA = 0x1908
        };
        enum TexWrap {
            Clamp = 0x2900,
            Repeat = 0x2901,
            ClampToEdge = 0x812F,
            ClampToBorder = 0x812D
        };
        enum TexFiltering {
            Nearest = 0x2600,
            Linear = 0x2601
        };

        Texture(const Texture& texture, bool copy_tex_data=false);

        Texture(
            std::string file_name, TexWrap tex_wrap = Repeat,
            TexFiltering tex_filtering = Linear,
            bool mipmap = true, float mipmap_bias = -1.0f,
            bool autoload = true
        );
        
        Texture(
            std::vector<uint8_t> data, int width, int height,
            TexType tex_type, TexWrap tex_wrap = Repeat,
            TexFiltering tex_filtering = Linear,
            bool mipmap = true, float mipmap_bias = -1.0f,
            bool autoload = true
        );
        ~Texture();
        bool Load(
            void(*API_loader)(Texture* texture_ptr),
            void(*API_unloader)(Texture* texture_ptr)
        );
        void Unload();
        uint32_t getTextureID();

        void setAutoload(bool value);
    };
}