#pragma once



#include "../PhysEngine.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/Mesh.hpp"
#include "../Objects/Model.hpp"
#include "../Objects/Particles.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/Camera.hpp"
#include "../Utilities/UUID.hpp"
#include "../UI/WidgetUI.hpp"

namespace HateEngine {
    class Level {
        friend class Engine;

    public:
        struct SceneUIWidget {
            WidgetUI* obj;
            bool is_ref;
        };
    private:
        PhysEngine physEngine;
        Camera *camera;

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
        Level();
        ~Level();

        void setCameraRef(Camera* camera);
        void removeCameraRef();

        PhysEngine* getPhysEngine();


        UUID addObjectClone(const Mesh& object, bool copy_tex=false);
        //UUID_Generator::UUID addObjectClone(Particles object);
        UUID addObjectClone(const Light& object);
        UUID addObjectClone(const Model& object, bool copy_tex=false);
        //UUID_Generator::UUID addObjectRef(Object* object);
        UUID addObjectRef(WidgetUI* object);
        UUID addObjectRef(Mesh* object);
        UUID addObjectRef(Light* object);
        UUID addObjectRef(Model* object);

        bool removeObject(const UUID& uuid);
    };
}