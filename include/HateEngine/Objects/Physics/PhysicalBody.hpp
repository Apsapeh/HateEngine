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

    public:
        enum BodyType { StaticBody, KinematicBody, DynamicBody, TriggerArea };

    private:
        struct ShapeObject {
            CollisionShape* shape;
            bool is_ref;
        };

        glm::vec3 prev_parent_position = {0, 0, 0};
        glm::mat4 prev_parent_rotation_matrix = glm::mat4(1.0f);
        std::unordered_map<UUID, ShapeObject> shapes;
        std::unordered_set<std::string> groups;

    public:
        reactphysics3d::RigidBody* reactRigidBody = nullptr;

    protected:
        BodyType bodyType;

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

        void setPosition(glm::vec3 vec);
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 vec);
        void setRotation(float x, float y, float z);

        void offset(float x, float y, float z);
        void offset(glm::vec3 vec);
        void rotate(float x, float y, float z);
        void rotate(glm::vec3 vec);

        void setLinearVelocity(glm::vec3 vec);
        void setLinearVelocity(float x, float y, float z);

        glm::vec3 getLinearVelocity() const;

        /**
         * Adds a collision shape to the body by copying the object
         * @param shape CollisionShape
         * @return Object ID
         */
        UUID addCollisionShapeClone(CollisionShape shape);

        /**
         * Adds a collision shape to the body by storing a pointer to the object
         * @param shape Pointer to CollisionShape
         * @return Object ID
         */
        UUID addCollisionShapeRef(CollisionShape* shape);

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
        std::unordered_map<UUID, ShapeObject> const* getShapes();


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
