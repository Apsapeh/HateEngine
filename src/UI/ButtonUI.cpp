#include <HateEngine/UI/ButtonUI.hpp>

using namespace HateEngine;

void on_click_wrapper(HateEngine::Engine* engine, void* data_p);

ButtonUI::ButtonUI(const ButtonUI& btn) : ObjectUI(btn) {
    this->text = btn.text;
    this->on_click = btn.on_click;
    this->color = btn.color;
    this->data_p = btn.data_p;
    //text_align = btn.text_align;
}

ButtonUI::ButtonUI(void (*on_click)(HateEngine::Engine* engine, void* data_p), void* data_p) : ObjectUI(Type::Button) {
    this->size = {100, 100};
    this->data_p = data_p;
    this->on_click = on_click;
}

ButtonUI::ButtonUI(void (*on_click)(HateEngine::Engine* engine)) : ObjectUI(Type::Button) {
    this->size = {100, 100};
    this->data_p = (void*)on_click;
    this->on_click = on_click_wrapper;
}

void ButtonUI::call_on_click(HateEngine::Engine* engine) const {
    if (on_click != nullptr) {
        on_click(engine, data_p);
    }
}

void on_click_wrapper(HateEngine::Engine* engine, void* data_p) {
    void (*on_click)(HateEngine::Engine* engine) = (void (*)(HateEngine::Engine* engine))data_p;
    on_click(engine);
}