#if defined(_WIN32)
#define STBI_NO_SIMD
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <stb_image.h>

#include <HateEngine/Log.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <vector>

#include "../globalStaticParams.hpp"

// Include OpenGL Utility header (glu.h)
#ifdef __linux__
#include <GL/glu.h>
#elif __APPLE__
#include <OpenGL/glu.h>
#elif _WIN32
#include <windows.h>
#include <GL/glu.h>
#endif

using namespace HateEngine;

Texture::Texture(const Texture& texture, bool copy_tex_data) {
    this->data = texture.data;
    this->width = texture.width;
    this->height = texture.height;
    this->textureFormat = texture.textureFormat;
    this->texWrap = texture.texWrap;
    this->texFiltering = texture.texFiltering;
    this->texMipMapFiltering = texture.texMipMapFiltering;
    this->MipMapLodBias = texture.MipMapLodBias;
    this->autoload = texture.autoload;
    this->fileName = texture.fileName;
    this->is_loaded = texture.is_loaded;
    this->is_gpu_loaded = texture.is_gpu_loaded;
    this->API_unloader = texture.API_unloader;

    if (copy_tex_data) {
        /*if (texture.textureGL_ID == 0 and this->autoload)
            Load();*/

        if (this->is_gpu_loaded) {
            glGenTextures(1, &this->textureGL_ID);
            glBindTexture(GL_TEXTURE_2D, this->textureGL_ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->texWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->texWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->texFiltering);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->texMipMapFiltering);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, this->MipMapLodBias);

            glBindTexture(GL_TEXTURE_2D, texture.textureGL_ID);
            int oldTextureChannelCount = texture.textureFormat == GL_RGBA ? 4 : 3;
            // Get the old texture data
            std::vector<GLubyte> cpy_data(
                    texture.width * texture.height * oldTextureChannelCount
            ); // Assuming RGBA format
            glGetTexImage(
                    GL_TEXTURE_2D, 0, texture.textureFormat, GL_UNSIGNED_BYTE, cpy_data.data()
            );

            // Bind the new texture and set the texture data
            glBindTexture(GL_TEXTURE_2D, this->textureGL_ID);

            if (this->texMipMapFiltering != this->texFiltering) // MipMap enabled
                gluBuild2DMipmaps(
                        GL_TEXTURE_2D, texture.textureFormat, texture.width, texture.height,
                        texture.textureFormat, GL_UNSIGNED_BYTE, cpy_data.data()
                );
            else // MipMad disabled
                glTexImage2D(
                        GL_TEXTURE_2D, 0, texture.textureFormat, texture.width, texture.height, 0,
                        texture.textureFormat, GL_UNSIGNED_BYTE, cpy_data.data()
                );
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    } else
        this->textureGL_ID = texture.textureGL_ID;
}

Texture::Texture(
        std::string file_name, Texture::TexWrap tex_wrap, Texture::TexFiltering tex_filtering,
        bool mipmap, float mipmap_bias, bool autoload
) {
    this->fileName = file_name;
    this->texWrap = tex_wrap;
    this->texFiltering = tex_filtering;
    this->texMipMapFiltering = tex_filtering;
    if (mipmap)
        this->texMipMapFiltering += GL_NEAREST_MIPMAP_LINEAR - GL_NEAREST;
    this->MipMapLodBias = mipmap_bias;
    this->autoload = autoload;

    this->is_loaded = loadFromFile();

    /*if (autoload)
        Load();*/
}

Texture::Texture(
        std::vector<uint8_t> data, int width, int height, Texture::TexType tex_type,
        Texture::TexWrap tex_wrap, Texture::TexFiltering tex_filtering, bool mipmap,
        float mipmap_bias, bool autoload
) {
    this->data = std::move(data);
    this->width = width;
    this->height = height;
    this->textureFormat = tex_type;
    this->texWrap = tex_wrap;
    this->texFiltering = tex_filtering;
    this->texMipMapFiltering = tex_filtering;
    if (mipmap)
        this->texMipMapFiltering += GL_NEAREST_MIPMAP_LINEAR - GL_NEAREST;
    this->MipMapLodBias = mipmap_bias;

    this->autoload = autoload;
    this->is_loaded = true;
    /*if (autoload)
        Load();*/
}

Texture::Texture(
        std::vector<uint8_t> data, TexWrap tex_wrap, TexFiltering tex_filtering, bool mipmap,
        float mipmap_bias
) {
    int n, width, height;
    Texture::TexType textureFormat = Texture::RGB;

    unsigned char* s_data = stbi_load_from_memory(data.data(), data.size(), &width, &height, &n, 0);
    if (s_data == nullptr) {
        HATE_ERROR("Texture: Error loading from memory")
        return;
    }
    if (n == 4)
        textureFormat = Texture::RGBA;
    this->data = std::vector<uint8_t>(s_data, s_data + width * height * n);
    stbi_image_free(s_data);

    this->width = width;
    this->height = height;
    this->textureFormat = textureFormat;
    this->texWrap = tex_wrap;
    this->texFiltering = tex_filtering;
    this->texMipMapFiltering = tex_filtering;
    if (mipmap)
        this->texMipMapFiltering += GL_NEAREST_MIPMAP_LINEAR - GL_NEAREST;
    this->MipMapLodBias = mipmap_bias;
    this->is_loaded = true;
}

Texture::~Texture() {
    Unload();
}

bool Texture::loadFromFile() {
    int n;
    unsigned char* s_data = stbi_load(this->fileName.c_str(), &this->width, &this->height, &n, 0);
    if (s_data == nullptr) {
        HATE_ERROR_F("Texture: Texture \"%s\" was not found", this->fileName.c_str());
        return false;
    }
    if (n == 4)
        this->textureFormat = GL_RGBA;
    this->data = std::vector<uint8_t>(s_data, s_data + this->width * this->height * n);
    stbi_image_free(s_data);
    return true;
}

bool Texture::Load(
        void (*API_load_func)(Texture* texture_ptr), void (*API_unloader)(Texture* texture_ptr)
) {
    this->API_unloader = API_unloader;
    if (this->API_unloader == nullptr) {
        HATE_WARNING("Error: API_unloader is not set");
        return false;
    }

    if (not this->is_loaded) {
        return false;
    }

    if (this->is_gpu_loaded) {
        this->data.clear();
        this->data.shrink_to_fit();
        return true;
    }

    if (!glad_is_initialized)
        return false;

    API_load_func(this);
    this->is_gpu_loaded = true;

    this->data.clear();
    this->data.shrink_to_fit();
    return true;
}

void Texture::Unload() {
    if (this->is_gpu_loaded) {
        this->API_unloader(this);
        is_gpu_loaded = false;
        autoload = false;
    }
}

uint32_t Texture::getTextureID() const {
    /*if (this->autoload)
        Load();*/

    return this->textureGL_ID;
}

int Texture::getWidth() const {
    return this->width;
}

int Texture::getHeight() const {
    return this->height;
}

void Texture::setAutoload(bool value) {
    this->autoload = value;
}
