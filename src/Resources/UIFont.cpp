#include <glad/gl.h>

#include <HateEngine/Log.hpp>
#include <HateEngine/Resources/UIFont.hpp>
#include <vector>
#include <fstream>

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

/*UIFont::UIFont(const UIFont& texture, bool copy_tex_data) {
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
    this->fileName = texture.fileName;

    if (copy_tex_data) {
        //if (texture.textureGL_ID == 0 and this->autoload)
        //    Load();

        if (this->is_loaded) {
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
}*/

#include <iostream>
bool UIFont::loadFromFile() {
    // read file

    std::ifstream file(this->fileName, std::ios::binary);
    if (!file.is_open()) {
        // HATE_ERROR("Error: Could not open file: %s", this->fileName.c_str());
        return false;
    }

    // get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // read file data
    // std::vector<char> file_data(file_size);
    this->data.resize(file_size);
    unsigned char* dt = this->data.data();
    file.read((char*) dt, file_size);
    file.close();
    return true;
}

UIFont::UIFont(std::string file_name, float pixel_height) {
    this->fileName = file_name;
    this->pixel_height = pixel_height;
    /*if (autoload)
        Load();*/
}

UIFont::~UIFont() {
    Unload();
}

bool UIFont::Load(
        void (*API_load_func)(UIFont* texture_ptr), void (*API_unloader)(UIFont* texture_ptr)
) {
    this->API_unloader = API_unloader;
    if (this->API_unloader == nullptr) {
        HATE_WARNING("Error: API_unloader is not set");
    }

    if (this->is_loaded) {
        this->data.clear();
        this->data.shrink_to_fit();
        return true;
    }

    if (!glad_is_initialized)
        return false;

    if (!this->fileName.empty() and not loadFromFile())
        return false;

    API_load_func(this);
    this->is_loaded = true;

    this->data.clear();
    this->data.shrink_to_fit();
    return true;
}

void UIFont::Unload() {
    if (this->is_loaded) {
        this->API_unloader(this);
        is_loaded = false;
    }
}

uint32_t UIFont::getTextureID() {
    /*if (this->autoload)
        Load();*/

    return this->textureGL_ID;
}

float UIFont::getPixelHeight() {
    return this->pixel_height;
}
