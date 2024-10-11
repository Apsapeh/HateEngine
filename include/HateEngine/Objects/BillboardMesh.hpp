#pragma once
#include "Mesh.hpp"

namespace HateEngine {
    class BillboardMesh : public Mesh {
        Object* target;

    public:
        BillboardMesh();
        void Update();
        void setSize(float width, float height);
        void setTarget(Object* obj);
        Object* getTarget();
    };
} // namespace HateEngine
