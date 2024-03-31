#pragma once

#include "../PhysEngine.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/Mesh.hpp"
#include "../Objects/Model.hpp"
#include "../Objects/Particles.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/Camera.hpp"
#include "../Utilities/UUID_Generator.hpp"

namespace HateEngine {
    class Level {
        friend class Engine;

        PhysEngine physEngine;
        Camera *camera;

        struct SceneObject {
            Object* obj;
            bool is_ref;
        };

        // 3D renderable objects
        std::unordered_map<UUID_Generator::UUID, SceneObject> meshes_obj;
        std::unordered_map<UUID_Generator::UUID, SceneObject> models_obj;
        std::unordered_map<UUID_Generator::UUID, SceneObject> particles_obj;
        std::unordered_map<UUID_Generator::UUID, SceneObject> lights_obj;

        // This vector should be generated from meshes_obj, models_obj
        std::vector<Mesh*> meshes;
        // This vector should be generated from particles_obj
        std::vector<Particles*> particles;
        // This vector should be generated from lights_obj
        std::vector<Light*> lights;

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

        UUID_Generator::UUID addObjectClone(const Mesh& object, bool copy_tex=false);
        //UUID_Generator::UUID addObjectClone(Particles object);
        UUID_Generator::UUID addObjectClone(const Light& object);
        UUID_Generator::UUID addObjectClone(const Model& object, bool copy_tex=false);
        //UUID_Generator::UUID addObjectRef(Object* object);
        UUID_Generator::UUID addObjectRef(Mesh* object);
        UUID_Generator::UUID addObjectRef(Light* object);
        UUID_Generator::UUID addObjectRef(Model* object);

        bool removeObject(const UUID_Generator::UUID& uuid);
    };
}

