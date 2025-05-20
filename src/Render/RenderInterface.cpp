#include <HateEngine/Render/RenderInterface.hpp>


namespace HateEngine {
    void RenderInterface::addLight(Light* light) {
        for (int i = 0; i < 8; ++i) {
            if (light->getMaskBit(i)) {
                lights[i].insert(std::make_pair(light->getUUID(), light));
            }
        }
        UUID uuid = light->onMaskChanged.connect([this](Light* light) { lightMaskChanged(light); });
        light_mask_changed_callbacks_uuids.insert(std::make_pair(light, uuid));
    }

    void RenderInterface::removeLight(Light* light) {
        light->onMaskChanged.disconnect(light_mask_changed_callbacks_uuids[light]);
        light_mask_changed_callbacks_uuids.erase(light);
        for (int i = 0; i < 8; ++i) {
            if (light->getMaskBit(i)) {
                lights[i].erase(light->getUUID());
            }
        }
    }

    void RenderInterface::lightMaskChanged(Light* light) {
        for (int i = 0; i < 8; ++i) {
            if (light->getMaskBit(i)) {
                lights[i].insert(std::make_pair(light->getUUID(), light));
            } else {
                lights[i].erase(light->getUUID());
            }
        }
    }

    double RenderInterface::getGPUTimeMS() {
        return double(last_gpu_time) / 10000000.0;
    }

    unsigned long RenderInterface::getDrawCalls() const {
        return last_draw_calls;
    }
} // namespace HateEngine
