#pragma once
#include <string>

namespace Old3DEngine {
    class Texture {
    private:
        std::string fileName;
        int texWrap;
        int texFiltering;
        bool autoload;
        uint32_t textureGL_ID = 0;

    public:
        enum TexWrap {
            Clamp = 0x2900,
            Repeat = 0x2901
        };
        enum TexFiltering {
            Nearest = 0x2600,
            Linear = 0x2601
        };

        Texture(std::string file_name, TexWrap tex_wrap, TexFiltering tex_filtering, bool autoload = true);
        ~Texture();
        bool Load();
        void Unload();
        uint32_t getTextureID();

        void setAutoload(bool value);
    };
}