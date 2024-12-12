//
// Created by Тимофей Кириченко on 12.05.2024.
//

#include <HateEngine/UI/ObjectUI.hpp>

using namespace HateEngine;

ObjectUI::ObjectUI(const ObjectUI& obj) {
    this->uuid = obj.uuid;
    this->position = obj.position;
    this->size = obj.size;
    this->type = obj.type;
    this->font = obj.font;
    this->text = obj.text;
    this->color = obj.color;
    this->text_color = obj.text_color;
    this->visible = obj.visible;
}

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

void ObjectUI::setFont(UIFont* font, bool autosize) {
    this->font = font;
    if (autosize) {
        this->size.y = font->getPixelHeight();
    }
}

void ObjectUI::removeFont() {
    this->font = nullptr;
}

UIFont* ObjectUI::getFont() const {
    return this->font;
}

void ObjectUI::setText(const std::string text) {
    this->text = text;
}

std::string ObjectUI::getText() const {
    return this->text;
}

ObjectUI::Type ObjectUI::getType() const {
    return this->type;
}

UUID ObjectUI::getUUID() const {
    return uuid;
}
