#pragma once
#include <cstdint>
#include <vector>
#include "../../Utilities/UUID_Generator.hpp"
#include "../Object.hpp"
#include "CollisionShape.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace Old3DEngine {
    class PhysicalBody : public Object {
    public:
        enum BodyType {
            StaticBody,
            RigidBody
        };

        struct CoordStruct {
            float x, y, z;
        };

        bool is_initialized = false;


    private:
        struct ShapeObject {
            CollisionShape *shape;
            UUID_Generator::UUID id;
            bool is_ref;
        };

        struct ControlledObject {
            Object* obj;
            CoordStruct offset;
            UUID_Generator::UUID id;
        };

        UUID_Generator uuidGenerator_shapes;
        UUID_Generator uuidGenerator_bindObj;
        std::vector<ShapeObject> shapes;
        std::vector<ControlledObject> binded_objects;
        reactphysics3d::RigidBody* reactRigidBody = nullptr;
        const reactphysics3d::PhysicsWorld* reactParentPhysWorld = nullptr;

    protected:
        BodyType bodyType;

    public:
        ~PhysicalBody();

        /**
         * Preserves the initialised physical body and physical world
         * @param parrent_world A pointer to the physical world that created the body
         * @param body A pointer to the physical body
         */
        void Init(reactphysics3d::PhysicsWorld* parrent_world, reactphysics3d::RigidBody* body);

        /**
         * Updates the position of linked objects taking into account the offset
         */
        void UpdateBindsPos();

        BodyType getBodyType();

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
        bool delCollisionShape(UUID_Generator::UUID uuid);


        /**
         * Binds the object to be moved along with the PhysicalBody
         * @param obj Pointer to Object
         * @param offset Displacement relative to the PhysicsBody position (x, y, z)
         * @return Object ID
         */
        UUID_Generator::UUID bindObjRef(Object* obj, CoordStruct offset = {0.0f, 0.0f, 0.0f});

        /**
         * Unbinds the object from moving with PhysicalBody
         * @param id Object ID
         * @return True if deleted successfully, False if the object is not found
         */
        bool unbindObj(UUID_Generator::UUID uuid);

        /**
         * Returns a pointer to the PhysicsCommon that created it, if the object is initialized
         * @return
         */
        const reactphysics3d::PhysicsWorld* getParentPhysCommon();
    };

}