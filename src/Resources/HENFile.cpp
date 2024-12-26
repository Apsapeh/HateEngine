#include <HateEngine/Resources/HENFile.hpp>
#include <HateEngine/Log.hpp>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace HateEngine;

HENFile::HENFile(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        HATE_ERROR_F("Error: Could not open file: %s", path.c_str());
        return;
    }

    this->load(
            [](char* ptr, long n, void* data_p) {
                std::ifstream* file = (std::ifstream*) data_p;
                file->read(ptr, n);
            },
            &file
    );
    file.close();
    this->is_loaded = true;
}

HENFile::HENFile(const char* data, uint32_t size) {
    const char** data_wrap = &data;
    this->load(
            [](char* ptr, long n, void* data_p) {
                const char** file = (const char**) data_p;
                std::memcpy(ptr, *file, n);
                *file += n;
            },
            data_wrap
    );
    this->is_loaded = true;
}

void HENFile::load(void (*get_bytes)(char* ptr, long n, void* data_p), void* data_p) {
    nodes = std::make_shared<std::vector<HENNode>>();
    uint32_t version;
    get_bytes((char*) &version, sizeof(version), data_p);

    uint32_t nodeCount;
    get_bytes((char*) &nodeCount, sizeof(nodeCount), data_p);

    this->nodes->reserve(nodeCount);
    // std::vector<

    for (uint64_t i = 0; i < nodeCount; i++) {
        HENNode node;
        // glm::vec3 pos;
        get_bytes((char*) &node.position, sizeof(node.position), data_p);
        uint32_t linkCount;
        get_bytes((char*) &linkCount, sizeof(linkCount), data_p);
        node.links.reserve(linkCount);
        this->nodes->push_back(std::move(node));
    }

    for (auto& node: *this->nodes) {
        for (uint64_t i = 0; i < node.links.capacity(); i++) {
            HENNodeLink link;
            uint32_t nodeIndex;
            get_bytes((char*) &nodeIndex, sizeof(nodeIndex), data_p);
            get_bytes((char*) &link.weight, sizeof(link.weight), data_p);
            link.node = &this->nodes->at(nodeIndex);
            node.links.push_back(std::move(link));
        }
    }
}

const std::vector<HENNode>* HENFile::getNodes() {
    return this->nodes.get();
}
