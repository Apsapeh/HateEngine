#pragma once
#include <vector>
#include "../Object.hpp"

namespace Old3DEngine {
    class CollisionShape : public Object {
    private:
        bool collisionLayers[16] = {
                1, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
        };
        bool collisionMasks[16]  = {
                1, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
        };


    public:
        enum ShapeEnum {
            Sphere,
            Capsule,
            Box
        };

        //              Sphere      Capsule     Box
        float param1;// radius      radius     width
        float param2;//             height     height
        float param3;//                        length
        ShapeEnum shapeType;

        /**
         * \~english Creating a collision shape with a specific shape
         * @param shape Type of collision shape
         * @param param1 Radius (Capsule, Sphere) or Width (Box)
         * @param param2 Height (Capsule, Box)
         * @param param3 Length (Box)
         */
        CollisionShape(ShapeEnum shape, float param1, float param2 = 0.0, float param3 = 0.0);

        /**
         * \~english Syntactic sugar to create a SPHERE collision shape
         * @param param1 Radius
         */
        CollisionShape(float radius);

        /**
         * \~english Syntactic sugar to create a CAPSULE collision shape
         * @param param1 Radius
         * @param param2 Height
         */
        CollisionShape(float radius, float height);

        /**
         * \~english Syntactic sugar to create a BOX collision shape
         * @param param1 Width
         * @param param2 Height
         * @param param3 Length
         */
        CollisionShape(float width, float height, float length);


        /**
         * Sets the collision layer on which a body can collide with other bodies
         * @param layers Array of layer numbers (0..15)
         * @param state Layer state array (true, false)
         */
        void setCollisionLayers(std::vector<uint8_t>& layers, std::vector<bool> state);

        /**
         * Sets the collision layer on which other bodies can collide with the body
         * @param masks Array of layer numbers (0..15)
         * @param state Layer state array (true, false)
         */
        void setCollisionMasks(std::vector<uint8_t>& masks, std::vector<bool> state);
    };
}