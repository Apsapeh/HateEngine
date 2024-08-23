//
// Created by Тимофей Кириченко on 12.05.2024.
//

#include <HateEngine/UI/LabelUI.hpp>

using namespace HateEngine;

LabelUI::LabelUI(const LabelUI& lbl) : ObjectUI(lbl) {
    this->color = lbl.color;
    this->text_align = lbl.text_align;
    this->text = lbl.text;
}

LabelUI::LabelUI() : ObjectUI(Type::Label) {
    this->size = {100, 100};
}
