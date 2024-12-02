#pragma once
#include "PhysicalBody.hpp"

namespace HateEngine {
    class TriggerArea : public PhysicalBody {
    private:
        void* on_enter_data_p = nullptr;
        void* on_exit_data_p = nullptr;
        void (*on_enter)(PhysicalBody* body, void* data_p);
        void (*on_exit)(PhysicalBody* body, void* data_p);

    public:
        TriggerArea();

        void setOnEnter(void (*on_enter)(PhysicalBody* body, void* data_p), void* data_p);
        void setOnExit(void (*on_exit)(PhysicalBody* body, void* data_p), void* data_p);

        void callOnEnter(PhysicalBody* body);
        void callOnExit(PhysicalBody* body);
    };
} // namespace HateEngine
