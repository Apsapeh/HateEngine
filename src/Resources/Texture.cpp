#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../Error.hpp"
#include "Texture.hpp"

using namespace Old3DEngine;
#include <iostream>
Texture::Texture(std::string file_name, Texture::TexWrap tex_wrap, Texture::TexFiltering tex_filtering, bool autoload) {
    if (!glfwInit()) {
        this->fileName = file_name;
        this->texWrap = tex_wrap;
        this->texFiltering = tex_filtering;
        this->autoload = autoload;
        return;
    }


}

Texture::~Texture() {
    if (this->textureGL_ID != 0)
        glDeleteTextures(1, &this->textureGL_ID);
}

bool Texture::Load() {
    if (!glfwInit())
        return false;

    int width, height, n;
    unsigned char *data = stbi_load(this->fileName.c_str(), &width, &height, &n, 0);
    if (data == nullptr) {
        Error::throwWarning("Error: Texture \"" + this->fileName + "\" was not found");
        return false;
    }

    glGenTextures(1, &this->textureGL_ID);
    glBindTexture(GL_TEXTURE_2D, this->textureGL_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->texFiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->texFiltering);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return true;
}
