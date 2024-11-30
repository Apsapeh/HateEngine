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
    elements.push_back({obj, true});
    return obj->getUUID();
}

UUID WidgetUI::addObjectClone(LabelUI& obj) {
    elements.push_back({new LabelUI(obj), false});
    return obj.getUUID();
}

UUID WidgetUI::addObjectClone(ButtonUI& obj) {
    elements.push_back({new ButtonUI(obj), false});
    return obj.getUUID();
}

UUID WidgetUI::addObjectClone(CheckboxUI& obj) {
    elements.push_back({new CheckboxUI(obj), false});
    return obj.getUUID();
}

UUID WidgetUI::addObjectClone(ImageUI& obj) {
    elements.push_back({new ImageUI(obj), false});
    return obj.getUUID();
}

bool WidgetUI::removeObjectRef(const UUID& uuid) {
    for (int i = 0; i < elements.size(); i++) {
        if (elements[i].obj->getUUID() == uuid) {
            if (!elements[i].is_ref) {
                switch (elements[i].obj->getType()) {
                    case ObjectUI::Type::Label:
                        delete (LabelUI*) elements[i].obj;
                        break;
                    case ObjectUI::Type::Button:
                        delete (ButtonUI*) elements[i].obj;
                        break;
                    case ObjectUI::Type::Checkbox:
                        delete (CheckboxUI*) elements[i].obj;
                        break;
                    case ObjectUI::Type::Image:
                        delete (ImageUI*) elements[i].obj;
                        break;
                }
            }
            elements.erase(elements.begin() + i);
            return true;
        }
    }
    return false;
}
