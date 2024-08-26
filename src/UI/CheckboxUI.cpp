#include <HateEngine/UI/CheckboxUI.hpp>

using namespace HateEngine;

CheckboxUI::CheckboxUI(const CheckboxUI& checkbox) : ObjectUI(checkbox) {
    this->is_checked = checkbox.is_checked;
}

CheckboxUI::CheckboxUI() : ObjectUI(Type::Checkbox) {
    this->size = {100, 100};
    // this->on_click = on_click;
}

void CheckboxUI::set_checked(bool checked) {
    this->is_checked = checked;
}

bool CheckboxUI::get_checked() {
    return this->is_checked;
}
