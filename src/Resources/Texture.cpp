#define STB_IMAGE_IMPLEMENTATION
#include <vector>
#include <stb_image.h>
#include <glad/gl.h>
#include <Old3DEngine/Error.hpp>
#include <Old3DEngine/Resources/Texture.hpp>
#include "../globalStaticParams.hpp"

using namespace Old3DEngine;

Texture::Texture(
    std::string file_name, Texture::TexWrap tex_wrap,
    Texture::TexFiltering tex_filtering, bool autoload
) {
    this->fileName = file_name;
    this->texWrap = tex_wrap;
    this->texFiltering = tex_filtering;
    this->autoload = autoload;
    if (autoload)
        Load();
}

Texture::Texture(
    std::vector<uint8_t> data, int width, int height,
    Texture::TexType tex_type, Texture::TexWrap tex_wrap,
    Texture::TexFiltering tex_filtering, bool autoload
) {
    this->data = std::move(data);
    this->width = width;
    this->height = height;
    this->textureFormat = tex_type;
    this->texWrap = tex_wrap;
    this->texFiltering = tex_filtering;
    this->autoload = autoload;
    if (autoload)
        Load();
}

Texture::~Texture() {
    Unload();
}

bool Texture::loadFromFile() {
    int n;
    unsigned char *s_data = stbi_load(this->fileName.c_str(), &this->width, &this->height, &n, 0);
    if (s_data == nullptr) {
        Error::throwWarning("Error: Texture \"" + this->fileName + "\" was not found");
        return false;
    }
    if (n == 4) this->textureFormat = GL_RGBA;
    this->data = std::vector<uint8_t>(s_data,  s_data + this->width * this->height * n);
    stbi_image_free(s_data);
    return true;
}

bool Texture::Load() {
    if (textureGL_ID != 0)
        return true;

    if (!glad_is_initialized)
        return false;

    if (!this->fileName.empty() and not loadFromFile())
        return false;

    glGenTextures(1, &this->textureGL_ID);
    glBindTexture(GL_TEXTURE_2D, this->textureGL_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->texFiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->texFiltering);
    glTexImage2D(GL_TEXTURE_2D, 0, this->textureFormat, width, height, 0, this->textureFormat, GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    this->data.clear();
    this->data.shrink_to_fit();
    return true;
}

void Texture::Unload() {
    if (this->textureGL_ID != 0) {
        glDeleteTextures(1, &this->textureGL_ID);
        textureGL_ID = 0;
        autoload = false;
    }
}

uint32_t Texture::getTextureID() {
    if (this->autoload)
        Load();

    return this->textureGL_ID;
}

void Texture::setAutoload(bool value) {
    this->autoload = value;
}
