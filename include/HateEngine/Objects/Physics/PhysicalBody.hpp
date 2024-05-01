#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>

#include "../../Utilities/UUID.hpp"
#include "../Object.hpp"
#include "CollisionShape.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace HateEngine {
    class PhysicalBody : public Object {
        friend class PhysEngine;
    public:
        enum BodyType {
            StaticBody,
            KinematicBody,
            DynamicBody
        };


    private:
        struct ShapeObject {
            CollisionShape *shape;
            bool is_ref;
        };

        glm::vec3 prev_parent_position = {0, 0, 0};
        glm::mat4 prev_parent_rotation_matrix = glm::mat4(1.0f);
        std::unordered_map<UUID, ShapeObject> shapes;
    public:
        reactphysics3d::RigidBody* reactRigidBody = nullptr;

    protected:
        BodyType bodyType;

    public:
        PhysicalBody(BodyType bodyType);

        ~PhysicalBody();

        /**
         * Preserves the initialised physical body and physical world
         * @param parrent_world A pointer to the physical world that created the body
         * @param body A pointer to the physical body
         */
        void Init(reactphysics3d::RigidBody* body);

        /**
         * Updates the position of body and linked objects taking into account the offset
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
         * Deletes the collision shape, if the object was cloned, the object will be completely deleted,
         * if a pointer to the object was stored, only the pointer will be deleted
         * @param id Object ID
         * @return True if deleted successfully, False if the object is not found
         */
        bool delCollisionShape(UUID uuid);

    };

}