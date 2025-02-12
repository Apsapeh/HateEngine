#pragma once
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <reactphysics3d/reactphysics3d.h>
#include "../../Utilities/UUID.hpp"
#include "../Object.hpp"
#include "CollisionShape.hpp"

namespace HateEngine {
    class PhysicalBody : public Object {
        friend class PhysEngine;
        friend class EventCallback;

    public:
        enum BodyType { StaticBody, KinematicBody, DynamicBody, TriggerArea };

        struct CollisionPoint {
            glm::vec3 point; // Global
            glm::vec3 normal;
        };

    protected:
        glm::vec3 prev_parent_position = {0, 0, 0};
        glm::mat4 prev_parent_rotation_matrix = glm::mat4(1.0f);
        std::unordered_map<UUID, CollisionShape*> shapes;
        std::unordered_set<std::string> groups;

        glm::vec3 linearVelocity = {0.0f, 0.0f, 0.0f};
        glm::vec3 angularVelocity = {0.0f, 0.0f, 0.0f};
        glm::vec3 angularLockAxisFactor = {1.0f, 1.0f, 1.0f};
        glm::vec3 forceLocal = {0.0f, 0.0f, 0.0f};
        glm::vec3 forceGlobal = {0.0f, 0.0f, 0.0f};
        float linearDamping = 0.0f;
        float angularDamping = 0.0f;
        float mass = 1.0f;
        bool isActive = true;
        bool isAllowedToSleep = true;
        bool isGravityEnabled = true;

        bool isRequiredCollisionPoints = false;
        std::unordered_map<PhysicalBody*, std::vector<CollisionPoint>> collisionPoints;

    public:
        reactphysics3d::RigidBody* reactRigidBody = nullptr;

    protected:
        BodyType bodyType;

        void updatePosition();
        void updateRotation();

    public:
        PhysicalBody(BodyType bodyType);

        ~PhysicalBody();

        /**
         * Preserves the initialised physical body and physical world
         * @param parrent_world A pointer to the physical world that created the
         * body
         * @param body A pointer to the physical body
         */
        void Init(reactphysics3d::RigidBody* body);

        /**
         * Updates the position of body and linked objects taking into account
         * the offset
         */
        void Update();

        BodyType getBodyType() const;

        void setParentPosition(const glm::vec3& vec) override;
        void setParentRotationMatrix(const glm::mat4& mat) override;
        void setPositionRaw(const glm::vec3& vec) override;
        void setRotationMatrixRaw(const glm::mat4& mat) override;

        void setLinearVelocity(float x, float y, float z);
        void setLinearVelocity(glm::vec3 vec);
        void setAngularVelocity(float x, float y, float z);
        void setAngularVelocity(glm::vec3 vec);
        void setAngularLockAxisFactor(float x, float y, float z);
        void setAngularLockAxisFactor(glm::vec3 vec);
        void applyForceLocal(float x, float y, float z);
        void applyForceLocal(const glm::vec3& vec);
        void applyForceGlobal(float x, float y, float z);
        void applyForceGlobal(const glm::vec3& vec);
        void setLinearDamping(float damping);
        void setAngularDamping(float damping);
        void setMass(float mass);
        void setIsActive(bool active);
        void setIsGravityEnabled(bool enabled);
        void setIsAllowedToSleep(bool allowed);

        void updateLocalCenterOfMassFromColliders();
        void resetForce();

        void setIsRequiredCollisionPoints(bool required);

        // glm::vec3 getForce() const;

        glm::vec3 getLinearVelocity() const;
        glm::vec3 getAngularVelocity() const;
        glm::vec3 getAngularLockAxisFactor() const;
        float getLinearDamping() const;
        float getAngularDamping() const;
        float getMass() const;
        bool getIsActive() const;
        bool getIsGravityEnabled() const;
        bool getIsAllowedToSleep() const;

        bool getIsRequiredCollisionPoints() const;

        /**
         * Adds a collision shape to the body by storing a pointer to the object
         * @param shape Pointer to CollisionShape
         * @return Object ID
         */
        UUID addCollisionShape(CollisionShape* shape);

        /**
         * Deletes the collision shape, if the object was cloned, the object
         * will be completely deleted, if a pointer to the object was stored,
         * only the pointer will be deleted
         * @param id Object ID
         * @return True if deleted successfully, False if the object is not
         * found
         */
        bool delCollisionShape(UUID uuid);

        /**
         * Gets a pointer to the collision shapes
         * @return A pointer to the collision shapes
         */
        std::unordered_map<UUID, CollisionShape*> const* getShapes();

        /**
         * Gets a pointer to the collision points
         * @return A pointer to the collision points
         */
        std::unordered_map<PhysicalBody*, std::vector<CollisionPoint>> const* const
        getCollisionPoints() const;


        /**
         * @brief Adds a group to the body. Just sugar for distinguishing bodies, like "enemy",
         * "friend", "collectable", etc.
         *
         * @param group
         */
        void addGroup(const std::string& group);
        void removeGroup(const std::string& group);
        bool hasGroup(const std::string& group);
        std::unordered_set<std::string> getGroups();
    };

} // namespace HateEngine
