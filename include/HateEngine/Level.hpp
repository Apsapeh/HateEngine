#pragma once

#include <unordered_map>
#include "Objects/Camera.hpp"
#include "Objects/Light/Light.hpp"
#include "Objects/Object.hpp"
#include "PhysEngine.hpp"
#include "UI/WidgetUI.hpp"
#include "Utilities/UUID.hpp"
#include "glm/ext/vector_float4.hpp"
#include "Objects/Interfaces/Renderable3DInterface.hpp"
#include "Objects/Interfaces/UpdatableInterface.hpp"

namespace HateEngine {
    class Level {
        friend class OpenGL_1_3;
        friend class Engine;

    public:
        enum FogMode { LINEAR, EXPONENTIAL, EXPONENTIAL_SQUARED };

    private:
        PhysEngine physEngine;
        Camera* camera = nullptr;

        /*   Settings   */
        FogMode fogMode = EXPONENTIAL;
        float fogDensity = 0.0f;
        float fogStart = 0.0f;
        float fogEnd = 0.0f;
        glm::vec4 fogColor = {0.5f, 0.5f, 0.5f, 1.0f};
        glm::vec4 backgroundColor = {0.5f, 0.5f, 0.5f, 0.0f};

        // Ambient light
        glm::vec4 calculatedAmbientLight = {0.5f, 0.5f, 0.5f, 1.0f};
        glm::vec3 ambientLightColor = {1.0f, 1.0f, 1.0f};
        float ambientLightIntensity = 1.0f;
        void updateAmbientLight(); // Recalculate the ambient light


        // UI
        std::unordered_map<UUID, WidgetUI*> ui_widgets;

        // 3D renderable objects
        std::unordered_map<UUID, Object*> objects;
        std::unordered_map<UUID, Light*> light;
        // Interfaces
        std::unordered_map<UUID, Renderable3DInterface*> renderable_objects;
        std::unordered_map<UUID, UpdatableInterface*> updatable_objects;
        

        // Functions
        /**
         * @brief This function is called by the engine to process the level
         * @param engine HateEngine. The engine that is calling the function
         */
        void (*processLoop)(void* engine, double delta) = nullptr;
        /**
         * @brief This function is called by the engine with a fixed time step
         * to process the level
         * @param engine HateEngine. The engine that is calling the function
         */
        void (*fixedProcessLoop)(void* engine, double delta) = nullptr;

        std::mutex uiWidgetsMutex;
        
        std::unordered_map<UUID, Light*>* getLights();
        // 2D renderable objects

    public:
        Level();
        ~Level();

        void Update(double delta);

        void setCameraRef(Camera* camera);
        Camera* getCameraRef();
        void removeCameraRef();

        /**
         * @brief This function is called by the engine to process the level
         * @param engine HateEngine. The engine that is calling the function
         */
        void setProcessLoop(void (*processLoop)(void* engine, double delta));
        /**
         * @brief This function is called by the engine with a fixed time step
         * to process the level
         * @param engine HateEngine. The engine that is calling the function
         */
        void setFixedProcessLoop(void (*fixedProcessLoop)(void* engine, double delta));

        PhysEngine* getPhysEngine();

        /*   Settings   */
        void setFogMode(FogMode mode);
        void setFogDensity(float density);
        void setFogStart(float start);
        void setFogEnd(float end);
        void setFogColor(glm::vec4 color);
        void setFogColor(float r, float g, float b, float a);
        void setBackgroundColor(glm::vec4 color);
        void setBackgroundColor(float r, float g, float b, float a);
        void setAmbientLightColor(glm::vec3 color);
        void setAmbientLightColor(float r, float g, float b);
        void setAmbientLightIntensity(float intensity);

        FogMode getFogMode() const;
        float getFogDensity() const;
        float getFogStart() const;
        float getFogEnd() const;
        glm::vec4 getFogColor() const;
        glm::vec4 getBackgroundColor() const;
        glm::vec3 getAmbientLightColor() const;
        float getAmbientLightIntensity() const;

        UUID addObject(WidgetUI* object);
        UUID addObject(Light* object);
        UUID addObject(Object* object);

        bool removeObject(const UUID& uuid);
    };
} // namespace HateEngine
