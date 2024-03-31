#pragma once
#include <vector>
#include <unordered_map>
#include <reactphysics3d/reactphysics3d.h>
#include "Objects/Physics/PhysicalBody.hpp"
#include "Utilities/UUID_Generator.hpp"

namespace HateEngine {
    class PhysEngine {
    public:
        struct PhysBodyObject {
            PhysicalBody* obj;
            bool is_ref;
        };

    private:
        reactphysics3d::PhysicsCommon physicsCommon;
        reactphysics3d::PhysicsWorld *physicsWorld;
        //std::vector<PhysBodyObject> physBodies;
        std::unordered_map<UUID_Generator::UUID, PhysBodyObject> physBodies;


    public:
        PhysEngine();

        /**
         * Updates the position of objects and checks for collision
         * @param delta Time elapsed since last update (sec)
         */
        void IteratePhysics(float delta);



        /**
         * Adds a physical body to the scene by copying it
         * @param object PhysicalBody
         * @return Object ID
         */
        UUID_Generator::UUID addObjectClone(const PhysicalBody& object);

        /**
         * Adds a physical body to the scene by pointer
         * @param PhysicalBody pointer
         * @return Object ID
         */
        UUID_Generator::UUID addObjectRef(PhysicalBody* object);

        /**
         * Deletes the physical body from the scene (if pointer, only the pointer will be deleted)
         * @param id Object ID
         * @return True if deleted successfully, False if the object is not found
         */
        bool removeObject(UUID_Generator::UUID uuid);
    };
}
