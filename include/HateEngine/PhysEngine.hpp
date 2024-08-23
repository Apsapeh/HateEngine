#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <unordered_map>
#include <vector>

#include "Objects/Physics/PhysicalBody.hpp"
#include "Utilities/UUID.hpp"

namespace HateEngine {
    class PhysEngine {
    public:
        struct PhysBodyObject {
            PhysicalBody* obj;
            bool is_ref;
        };

    private:
        static reactphysics3d::PhysicsCommon physicsCommon;
        reactphysics3d::PhysicsWorld* physicsWorld;
        UUID uuid;
        // std::vector<PhysBodyObject> physBodies;
        std::unordered_map<UUID, PhysBodyObject> physBodies;

    public:
        PhysEngine();
        
        ~PhysEngine();

        /**
         * Updates the position of objects and checks for collision
         * @param delta Time elapsed since last update (sec)
         */
        void IteratePhysics(float delta);

        //!======================> React WorldSettings <======================
        /**
         * Gets the physics world
         * @return A pointer to the physics world
         */
        const reactphysics3d::PhysicsWorld* getPhysicsWorld() const;

        /**
         * Adds a physical body to the scene by copying it
         * @param object PhysicalBody
         * @return Object ID
         */
        UUID addObjectClone(const PhysicalBody& object);

        /**
         * Adds a physical body to the scene by pointer
         * @param PhysicalBody pointer
         * @return Object ID
         */
        UUID addObjectRef(PhysicalBody* object);

        /**
         * Deletes the physical body from the scene (if pointer, only the
         * pointer will be deleted)
         * @param id Object ID
         * @return True if deleted successfully, False if the object is not
         * found
         */
        bool removeObject(UUID uuid);
    };
} // namespace HateEngine
