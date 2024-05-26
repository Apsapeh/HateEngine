//
// Created by Тимофей Кириченко on 12.05.2024.
//

#include <HateEngine/UI/ObjectUI.hpp>

using namespace HateEngine;

ObjectUI::ObjectUI(Type type) {
    this->type = type;
}

UUID ObjectUI::getUUID() const {
    return uuid;
}
