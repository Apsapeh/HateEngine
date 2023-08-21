#pragma once
#include <cstdint>
#include <vector>
#include "../../Utilities/UUID_Generator.hpp"
#include "../Object.hpp"
#include "CollisionShape.hpp"

namespace Old3DEngine {
    class PhysicalBody : public Object {
    public:
        enum BodyType {
            StaticBody,
            RigidBody
        };

    private:
        struct ShapeObject {
            CollisionShape *shape;
            UUID_Generator::UUID id;
            bool is_ref;
        };

        UUID_Generator generator;

    protected:
        BodyType bType;

    public:
        ~PhysicalBody();

        /**
         * Adds a collision shape to the body by copying the object
         * @param shape CollisionShape
         * @return Object ID
         */
        UUID_Generator::UUID addCollisionShapeClone(CollisionShape shape);

        /**
         * Adds a collision shape to the body by storing a pointer to the object
         * @param shape Pointer to CollisionShape
         * @return Object ID
         */
        UUID_Generator::UUID addCollisionShapeRef(CollisionShape* shape);

        /**
         * Deletes the collision shape, if the object was cloned, the object will be completely deleted,
         * if a pointer to the object was stored, only the pointer will be deleted
         * @param id Object ID
         * @return True if deleted successfully, False if the object is not found
         */
        bool delCollisionShape(UUID_Generator::UUID id);
    };

}