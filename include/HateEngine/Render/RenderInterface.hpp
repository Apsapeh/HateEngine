#pragma once

#include <unordered_map>
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
        long gpu_time = 0.0;
        // Render time in nanoseconds
        long last_gpu_time = 0.0;

        // Pointer to map of lights for render, use 'setLightsRef'
        std::unordered_map<UUID, Light*>* lights = nullptr;

    public:
        virtual void Render() = 0;
        virtual void renderMesh(const class Mesh* mesh) = 0;

        virtual class Camera* getCamera() = 0;

        /**
         * @brief Set the lights map for render
         * @param lights Map of lights
         */
        void setLightsRef(std::unordered_map<UUID, Light*>* lights);

        /**
         * @brief Return the GPU time in milliseconds
         * @warning Only for 32-bit systems. If GPU rendering takes more, then 2.147 seconds, the
         * result will be incorrect (long overflow)
         *
         * @return double
         */
        double getGPUTimeMS();
    };
} // namespace HateEngine
