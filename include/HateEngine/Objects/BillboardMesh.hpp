#pragma once
#include "Mesh.hpp"
#include "Interfaces/Renderable3DInterface.hpp"
#include "Interfaces/UpdatableInterface.hpp"

namespace HateEngine {
    class BillboardMesh : public Mesh, public UpdatableInterface {
        Object* target;

    public:
        BillboardMesh();
        void setSize(float width, float height);
        void setTarget(Object* obj);
        Object* getTarget();

        void _fixedProcess(class Engine* engine, double delta) override;
    };
} // namespace HateEngine
