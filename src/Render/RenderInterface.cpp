#include <HateEngine/Render/RenderInterface.hpp>

namespace HateEngine {
    void RenderInterface::setLightsRef(std::unordered_map<UUID, Light*>* lights) {
        this->lights = lights;
    }

    double RenderInterface::getGPUTimeMS() {
        return double(last_gpu_time) / 10000000.0;
    }
} // namespace HateEngine
