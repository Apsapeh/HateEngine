#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace HateEngine {
    class OpenGL15;

    class UIFont {
        friend OpenGL15;

    private:
        std::string fileName;
        float pixel_height = 18;
        std::vector<uint8_t> data = {};
        uint32_t textureGL_ID = 0;
        bool is_loaded = false;
        bool is_unable_to_load = false;
        void* render_api_data = nullptr;

        bool loadFromFile();


        void (*API_unloader)(UIFont*);


    public:
        // UIFont(const UIFont& texture, bool copy_tex_data = false);

        UIFont(std::string file_name, float pixel_height = 18);

        // UIFont(std::vector<uint8_t> data);
        ~UIFont();
        bool Load(
                void (*API_loader)(UIFont* texture_ptr), void (*API_unloader)(UIFont* texture_ptr)
        );
        void Unload();

        uint32_t getTextureID();
        float getPixelHeight();
    };
} // namespace HateEngine
