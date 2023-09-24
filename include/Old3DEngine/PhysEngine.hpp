#pragma once
#include <vector>
#include <reactphysics3d/reactphysics3d.h>
#include "Objects/Physics/PhysicalBody.hpp"
#include "Utilities/UUID_Generator.hpp"

namespace Old3DEngine {
    class PhysEngine {
    public:
        struct physBodyStruct {
            UUID_Generator::UUID uuid;
            PhysicalBody* obj;
        };

    private:
        UUID_Generator uuidGenerator;
        reactphysics3d::PhysicsCommon physicsCommon;
        reactphysics3d::PhysicsWorld *physicsWorld;
        std::vector<physBodyStruct> physBodies;

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
        UUID_Generator::UUID addObjectClone(PhysicalBody object);

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
