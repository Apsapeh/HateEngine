#include <blowfish/blowfish.h>
#include <stb_image.h>

#include <HateEngine/Resources/HERFile.hpp>
#include <fstream>
#include <iostream>
#include "HateEngine/Log.hpp"

using namespace HateEngine;

std::vector<uint8_t>* HERResource::getRawData() {
    return &this->data;
}

Texture HERResource::asTexture(
        Texture::TexWrap tex_wrap, Texture::TexFiltering tex_filtering, bool mipmap,
        float mipmap_bias, bool autoload
) {
    // return Texture(this->data, this->width, this->height);
    int n, width, height;
    Texture::TexType textureFormat = Texture::RGB;

    unsigned char* s_data =
            stbi_load_from_memory(this->data.data(), this->data.size(), &width, &height, &n, 0);
    if (n == 4)
        textureFormat = Texture::RGBA;
    this->data = std::vector<uint8_t>(s_data, s_data + width * height * n);
    stbi_image_free(s_data);
    return Texture(
            this->data, width, height, textureFormat, tex_wrap, tex_filtering, mipmap, mipmap_bias
    );
}

GLTFModel HERResource::asGLBModel() {
    return GLTFModel(this->data.data(), this->data.size());
}

HERFile::HERFile(std::string path, std::string password) {
    this->path = path;
    this->password = password;

    std::ifstream file(path, std::ios::binary);
    uint32_t version;
    file.read((char*) &version, sizeof(version));

    file.read((char*) &this->dataPointer, sizeof(this->dataPointer));

    uint64_t resourceCount;
    file.read((char*) &resourceCount, sizeof(resourceCount));

    this->resources.reserve(resourceCount);
    for (uint64_t i = 0; i < resourceCount; i++) {
        HERResourceRaw resource;
        uint64_t nameLength;
        file.read((char*) &nameLength, sizeof(nameLength));
        std::string name;
        name.resize(nameLength);
        file.read(&name[0], nameLength);

        file.read((char*) &resource.originalSize, sizeof(resource.originalSize));
        file.read((char*) &resource.alignedSize, sizeof(resource.alignedSize));
        file.read((char*) &resource.offset, sizeof(resource.offset));
        this->resources[name] = resource;
    }

    // unsigned char key[] = "password";

    std::cout << "Version: " << version << std::endl;
    std::cout << "Data pointer: " << dataPointer << std::endl;
    std::cout << "Resource count: " << resourceCount << std::endl;

    std::cout << "\n\nResources:" << std::endl;
    for (const auto& resource_pair: resources) {
        const HERResourceRaw& resource = resource_pair.second;
        std::cout << "\tResource: " << resource_pair.first << std::endl;
        std::cout << "\tName length: " << resource_pair.first.size() << std::endl;
        std::cout << "\tOriginal size: " << resource.originalSize << std::endl;
        std::cout << "\tAligned size: " << resource.alignedSize << std::endl;
        std::cout << "\tOffset: " << resource.offset << std::endl << std::endl;
    }
    file.close();
}

HERResource HERFile::operator[](std::string key) {
    std::ifstream file(path, std::ios::binary);
    Blowfish blowfish;
    blowfish.SetKey((const unsigned char*) password.c_str(), password.size());

    HERResource result;
    HERResourceRaw& resource = this->resources[key];
    file.seekg(resource.offset + this->dataPointer);
    // std::vector<unsigned char> data(resource.alignedSize);
    result.data.resize(resource.alignedSize);
    file.read((char*) &result.data[0], resource.alignedSize);

    blowfish.Decrypt(&result.data[0], &result.data[0], resource.alignedSize);
    result.data.resize(resource.originalSize);

    file.close();
    return result;
}
