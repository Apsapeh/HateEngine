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


#include <iostream>
bool UIFont::loadFromFile() {
    std::ifstream file(this->fileName, std::ios::binary);
    if (!file.is_open()) {
        // HATE_ERROR("Error: Could not open file: %s", this->fileName.c_str());
        HATE_ERROR_F("UIFont: Could not open file: %s", this->fileName.c_str());
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
    this->is_loaded = true;
    return true;
}

UIFont::UIFont(std::string file_name, float pixel_height) {
    this->fileName = file_name;
    this->pixel_height = pixel_height;
    loadFromFile();
    /*if (autoload)
        Load();*/
}

UIFont::~UIFont() {
    Unload();
}

bool UIFont::Load(
        void (*API_load_func)(UIFont* texture_ptr), void (*API_unloader)(UIFont* texture_ptr)
) {
    if (!this->is_loaded) {
        return false;
    }

    this->API_unloader = API_unloader;
    if (this->API_unloader == nullptr) {
        HATE_WARNING("Error: API_unloader is not set");
    }

    if (this->is_gpu_loaded) {
        this->data.clear();
        this->data.shrink_to_fit();
        return true;
    }

    if (!glad_is_initialized)
        return false;

    if (!this->fileName.empty() and not loadFromFile())
        return false;

    API_load_func(this);
    this->is_gpu_loaded = true;

    this->data.clear();
    this->data.shrink_to_fit();
    return true;
}

void UIFont::Unload() {
    if (this->is_gpu_loaded) {
        this->API_unloader(this);
        is_gpu_loaded = false;
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

bool UIFont::isGPULoaded() {
    return this->is_gpu_loaded;
}
