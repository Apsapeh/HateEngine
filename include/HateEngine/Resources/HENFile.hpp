#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Resource.hpp"

namespace HateEngine {
    struct HENNodeLink {
        float weight;
        struct HENNode* node;
    };

    struct HENNode {
        glm::vec3 position;
        std::vector<HENNodeLink> links;
    };

    class HENFile : public Resource {
    private:
        std::shared_ptr<std::vector<HENNode>> nodes;

        void load(void (*get_bytes)(char* ptr, long n, void* data_p), void* data_p);

    public:
        HENFile(std::string path);
        HENFile(const char* data, uint32_t size);

        const std::vector<HENNode>* getNodes();
    };
} // namespace HateEngine
