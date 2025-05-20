#pragma once

#include <unordered_map>
#include <array>
#include "../Utilities/UUID.hpp"
#include "../Objects/Light/Light.hpp"

/*
    Interface is very unstable (like all in this engine) and may be changed in a future commits
*/


namespace HateEngine {
    class RenderInterface {
        friend class Engine;

    protected:
        // Render time in nanoseconds. While render you increase this value, after swap with
        // last_gpu_time, after set to 0
        long gpu_time = 0;
        // Render time in nanoseconds
        long last_gpu_time = 0;

        unsigned long draw_calls = 0;
        unsigned long last_draw_calls = 0;

        // Sorted array of lights by mask
        std::array<std::unordered_map<UUID, Light*>, 8> lights = {};
        std::unordered_map<Light*, UUID> light_mask_changed_callbacks_uuids = {};

        void lightMaskChanged(Light* light);

    public:
        virtual void Render() = 0;
        virtual void renderMesh(const class Mesh* mesh) = 0;

        virtual class Camera* getCamera() = 0;

        /**
         * @brief Add light to rendering server
         *
         * @param light
         */
        void addLight(Light* light);

        /**
         * @brief Remove light from rendering server
         *
         * @param light
         */
        void removeLight(Light* light);

        /**
         * @brief Return the GPU time in milliseconds
         * @warning Only for 32-bit systems. If GPU rendering takes more, then 2.147 seconds, the
         * result will be incorrect (long overflow)
         *
         * @return double
         */
        double getGPUTimeMS();

        /**
         * @brief Get the draw calls
         *
         * @return unsigned long
         */
        unsigned long getDrawCalls() const;
    };
} // namespace HateEngine
