#include <HateEngine/UI/ButtonUI.hpp>

using namespace HateEngine;
ButtonUI::ButtonUI(void (*on_click)(HateEngine::Engine* engine)) : ObjectUI(Type::Button) {
    this->size = {100, 100};
    this->on_click = on_click;
}