#pragma once
#include <vector>
#include "Objects/Physics/PhysicalBody.hpp"

namespace Old3DEngine {
    class PhysEngine {
    private:
        std::vector<PhysicalBody> *physBodies;

    public:
        PhysEngine(std::vector<PhysicalBody> *phys_bodies_vec);

        void IteratePhysics();
    };
}
