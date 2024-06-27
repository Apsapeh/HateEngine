//
// Created by Тимофей Кириченко on 12.05.2024.
//

#include <HateEngine/UI/ObjectUI.hpp>

using namespace HateEngine;

ObjectUI::ObjectUI(Type type) {
    this->type = type;
}

void ObjectUI::setScale(float scale) {
    this->size.scale = scale;
    this->position.scale = scale;
}

void ObjectUI::zoom(float delta) {
    this->size.scale += delta;
    this->position.scale += delta;
}

UUID ObjectUI::getUUID() const {
    return uuid;
}
