//
// Created by Тимофей Кириченко on 12.05.2024.
//
#include <HateEngine/UI/WidgetUI.hpp>

using namespace HateEngine;

WidgetUI::WidgetUI() {

}

UUID WidgetUI::addObjectRef(ObjectUI *obj) {
    elements[obj->getUUID()] = {obj, true};
    return obj->getUUID();
}

bool WidgetUI::removeObjectRef(const UUID &uuid) {
    if (elements.count(uuid) == 1) {
        if (!elements[uuid].is_ref)
            delete elements[uuid].obj;
        elements.erase(uuid);
        return true;
    }
}

