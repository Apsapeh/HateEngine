#include <HateEngine/UI/CheckboxUI.hpp>

using namespace HateEngine;
CheckboxUI::CheckboxUI() : ObjectUI(Type::CheckboxUI) {
    this->size = {100, 100};
    //this->on_click = on_click;
}

void CheckboxUI::set_checked(bool checked) {
    this->is_checked = checked;
}

bool CheckboxUI::get_checked() const {
    return this->is_checked;
}