#pragma once
#include <cstdint>
#include <vector>
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

    protected:
        BodyType bType;

    public:

    };

}