#pragma once

#include "../Objects/Camera.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/Mesh.hpp"
#include "../Objects/Model.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/Particles.hpp"
#include "../PhysEngine.hpp"
#include "../UI/WidgetUI.hpp"
#include "../Utilities/UUID.hpp"

namespace HateEngine {
    class Level {
        friend class Engine;
        friend class OpenGL15;

    public:
        enum FogMode { LINEAR, EXPONENTIAL, EXPONENTIAL_SQUARED };
        struct LevelSettings {
            FogMode fog_mode = EXPONENTIAL;
            float fog_density = 0.0f;
            float fog_start = 0.0f;
            float fog_end = 0.0f;
            float fog_color[4] = {0.5f, 0.5f, 0.5f, 1.0f};
            float background_color[4] = {0.5f, 0.5f, 0.5f, 0.0f};
        };

        struct SceneUIWidget {
            WidgetUI* obj;
            bool is_ref;
        };

    private:
        PhysEngine physEngine;
        Camera* camera = nullptr;

        struct SceneObject {
            Object* obj;
            bool is_ref;
        };

        // UI
        std::unordered_map<UUID, SceneUIWidget> ui_widgets;

        // 3D renderable objects
        std::unordered_map<UUID, SceneObject> meshes_obj;
        std::unordered_map<UUID, SceneObject> models_obj;
        std::unordered_map<UUID, SceneObject> particles_obj;
        std::unordered_map<UUID, SceneObject> lights_obj;

        // This vector should be generated from meshes_obj, models_obj
        std::vector<Mesh*> meshes;
        // This vector should be generated from particles_obj
        std::vector<Particles*> particles;
        // This vector should be generated from lights_obj
        std::vector<Light*> lights;

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
        std::mutex meshesMutex;
        std::mutex modelsMutex;
        std::mutex particlesMutex;
        std::mutex lightsMutex;

        void updateMeshesVector();
        void updateParticlesVector();
        void updateLightsVector();

        // 2D renderable objects

    public:
        LevelSettings settings;

        Level();
        ~Level();

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

        UUID addObjectClone(const Mesh& object, bool copy_tex = false);
        // UUID_Generator::UUID addObjectClone(Particles object);
        UUID addObjectClone(const Light& object);
        UUID addObjectClone(const Model& object, bool copy_tex = false);
        // UUID_Generator::UUID addObjectRef(Object* object);
        UUID addObjectRef(WidgetUI* object);
        UUID addObjectRef(Mesh* object);
        UUID addObjectRef(Light* object);
        UUID addObjectRef(Model* object);

        bool removeObject(const UUID& uuid);
    };
} // namespace HateEngine
