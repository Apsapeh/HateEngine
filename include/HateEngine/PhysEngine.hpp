#pragma once
#include <cstdint>
#include <reactphysics3d/reactphysics3d.h>
#include <unordered_map>

#include "Objects/Physics/PhysicalBody.hpp"
#include "Utilities/UUID.hpp"
#include "reactphysics3d/collision/RaycastInfo.h"
#include "reactphysics3d/engine/EventListener.h"

namespace HateEngine {
    class PhysEngine {
        friend class RayCast;
        friend class RayCastCallback;

    private:
        static reactphysics3d::PhysicsCommon* physicsCommon;
        reactphysics3d::PhysicsWorld* physicsWorld;
        reactphysics3d::EventListener* listener;
        UUID uuid;
        // std::vector<PhysBodyObject> physBodies;
        std::unordered_map<UUID, PhysicalBody*> physBodies;

    protected:
        void getRayCastCollisions(
                glm::vec3 startPos, glm::vec3 endPos, reactphysics3d::RaycastCallback* callback,
                uint16_t mask
        );

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
         * @brief Enable/Disable gravity
         *
         * @param isGravityEnabled
         */
        void setIsGravityEnabled(bool isGravityEnabled);

        /**
         * @brief Set the gravity
         *
         * @param gravity
         */
        void setGravity(const glm::vec3& gravity);

        /**
         * @brief Set the number of iterations of the position solver
         *
         * @param count
         */
        void setIterationsPositionSolver(uint32_t count);

        /**
         * @brief Set the number of iterations of the velocity solver
         *
         * @param count
         */
        void setIterationsVelocitySolver(uint32_t count);


        /**
         * @brief Get the gravity state
         *
         * @return True if gravity is enabled, False otherwise
         */
        bool isGravityEnabled();

        /**
         * @brief Get the gravity
         *
         * @return glm::vec3
         */
        glm::vec3 getGravity();

        /**
         * @brief Get the number of iterations of the position solver
         *
         * @return uint32_t
         */
        uint32_t getIterationsPositionSolver();

        /**
         * @brief Get the number of iterations of the velocity solver
         *
         * @return uint32_t
         */
        uint32_t getNbIterationsVelocitySolver();

        /**
         * Adds a physical body to the scene by pointer
         * @param PhysicalBody pointer
         * @return Object ID
         */
        UUID addObject(PhysicalBody* object);

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
