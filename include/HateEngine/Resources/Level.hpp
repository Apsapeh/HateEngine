#pragma once

#include "../Objects/Camera.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/Mesh.hpp"
#include "../Objects/Model.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/BillboardMesh.hpp"
#include "../Objects/Particles.hpp"
#include "../PhysEngine.hpp"
#include "../UI/WidgetUI.hpp"
#include "../Utilities/UUID.hpp"
#include "HateEngine/Objects/GLTFAnimationPlayer.hpp"
#include "glm/ext/vector_float4.hpp"

namespace HateEngine {
    class Level {
        friend class Engine;
        friend class OpenGL15;

    public:
        enum FogMode { LINEAR, EXPONENTIAL, EXPONENTIAL_SQUARED };

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

        /*   Settings   */
        FogMode fogMode = EXPONENTIAL;
        float fogDensity = 0.0f;
        float fogStart = 0.0f;
        float fogEnd = 0.0f;
        glm::vec4 fogColor = {0.5f, 0.5f, 0.5f, 1.0f};
        glm::vec4 backgroundColor = {0.5f, 0.5f, 0.5f, 0.0f};

        // Ambient light
        glm::vec4 calculatedAmbientLight = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec3 ambientLightColor = {1.0f, 1.0f, 1.0f};
        float ambientLightIntensity = 1.0f;
        void updateAmbientLight(); // Recalculate the ambient light

        // UI
        std::unordered_map<UUID, SceneUIWidget> ui_widgets;

        // 3D renderable objects
        std::unordered_map<UUID, SceneObject> meshes_obj;
        std::unordered_map<UUID, SceneObject> models_obj;
        std::unordered_map<UUID, SceneObject> billboards_obj;
        std::unordered_map<UUID, SceneObject> animationPlayers_obj;
        std::unordered_map<UUID, SceneObject> particles_obj;
        std::unordered_map<UUID, SceneObject> lights_obj;

        // This vector should be generated from meshes_obj, models_obj
        std::vector<Mesh*> meshes;
        // This vector should be generated from animationPlayers_obj
        std::vector<GLTFAnimationPlayer*> animationPlayers;
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
        std::mutex billboardsMutex;
        std::mutex animationPlayersMutex;
        std::mutex particlesMutex;
        std::mutex lightsMutex;

        void updateMeshesVector();
        void updateAnimationPlayersVector();
        void updateParticlesVector();
        void updateLightsVector();

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

        FogMode getFogMode();
        float getFogDensity();
        float getFogStart();
        float getFogEnd();
        glm::vec4 getFogColor();
        glm::vec4 getBackgroundColor();
        glm::vec3 getAmbientLightColor();
        float getAmbientLightIntensity();


        UUID addObjectClone(const Mesh& object, bool copy_tex = false);
        // UUID_Generator::UUID addObjectClone(Particles object);
        UUID addObjectClone(const Light& object);
        UUID addObjectClone(const Model& object, bool copy_tex = false);
        // UUID_Generator::UUID addObjectRef(Object* object);
        UUID addObjectRef(WidgetUI* object);
        UUID addObjectRef(Mesh* object);
        UUID addObjectRef(BillboardMesh* object);
        UUID addObjectRef(Light* object);
        UUID addObjectRef(Model* object);
        UUID addObjectRef(GLTFAnimationPlayer* object);
        UUID addObjectRef(Particles* object);

        bool removeObject(const UUID& uuid);
    };
} // namespace HateEngine
