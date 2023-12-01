#pragma once
#include <string>

namespace Old3DEngine {
    class Texture {
    private:
        std::vector<uint8_t> data = {};
        int width = 0;
        int height = 0;
        int textureFormat = 0x1907;
        std::string fileName;
        int texWrap;
        int texFiltering;
        bool autoload;
        uint32_t textureGL_ID = 0;

        bool loadFromFile();

    public:
        enum TexType {
            RGB = 0x1907,
            RGBA = 0x1908
        };
        enum TexWrap {
            Clamp = 0x2900,
            Repeat = 0x2901
        };
        enum TexFiltering {
            Nearest = 0x2600,
            Linear = 0x2601
        };

        Texture(
            std::string file_name, TexWrap tex_wrap,
            TexFiltering tex_filtering, bool autoload = true
        );
        Texture(std::vector<uint8_t> data, int width, int height,
            TexType tex_type, TexWrap tex_wrap,
            TexFiltering tex_filtering, bool autoload = true
        );
        ~Texture();
        bool Load();
        void Unload();
        uint32_t getTextureID();

        void setAutoload(bool value);
    };
}