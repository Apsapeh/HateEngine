//
// Created by Тимофей Кириченко on 12.05.2024.
//
#include <HateEngine/UI/WidgetUI.hpp>
#include <string>

// #include "HateEngine/UI/CheckboxUI.hpp"
#include "HateEngine/UI/ObjectUI.hpp"

using namespace HateEngine;

WidgetUI::WidgetUI() : ObjectUI(Type::Widget) {
    this->size = {100, 100};
    this->title += std::to_string(this->getUUID().getU64());
}

UUID WidgetUI::addObjectRef(ObjectUI* obj) {
    elements[obj->getUUID()] = {obj, true};
    return obj->getUUID();
}

UUID WidgetUI::addObjectClone(LabelUI& obj) {
    elements[obj.getUUID()] = {new LabelUI(obj), false};
    return obj.getUUID();
}

UUID WidgetUI::addObjectClone(ButtonUI& obj) {
    elements[obj.getUUID()] = {new ButtonUI(obj), false};
    return obj.getUUID();
}

UUID WidgetUI::addObjectClone(CheckboxUI& obj) {
    elements[obj.getUUID()] = {new CheckboxUI(obj), false};
    return obj.getUUID();
}

bool WidgetUI::removeObjectRef(const UUID& uuid) {
    if (elements.count(uuid) == 1) {
        if (!elements[uuid].is_ref)
            delete elements[uuid].obj;
        elements.erase(uuid);
        return true;
    }
    return false;
}
