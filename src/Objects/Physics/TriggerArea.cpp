#include <HateEngine/Objects/Physics/TriggerArea.hpp>
#include "HateEngine/Log.hpp"

using namespace HateEngine;

TriggerArea::TriggerArea() : PhysicalBody(BodyType::TriggerArea) {
}

void TriggerArea::setOnEnter(void (*on_enter)(PhysicalBody* body, void* data_p), void* data_p) {
    this->on_enter = on_enter;
    this->on_enter_data_p = data_p;
}

void TriggerArea::setOnExit(void (*on_exit)(PhysicalBody* body, void* data_p), void* data_p) {
    this->on_exit = on_exit;
    this->on_exit_data_p = data_p;
}


void TriggerArea::callOnEnter(PhysicalBody* body) {
    if (on_enter != nullptr)
        on_enter(body, on_enter_data_p);
}

void TriggerArea::callOnExit(PhysicalBody* body) {
    if (on_exit != nullptr)
        on_exit(body, on_exit_data_p);
}
